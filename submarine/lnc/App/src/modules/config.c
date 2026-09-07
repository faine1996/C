#include "config.h"
#include "comm.h"
#include "test_bench.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include <string.h>
#include <stdio.h>

/* -----------------------------------------------------------------------
 * Flash page 511 — last page of Bank 2.
 * Address: 0x080FF800, size: 2KB.
 * Safe to use: far from code, never touched by the linker.
 * Bank 2 starts at 0x08080000; page 511 is page 255 within Bank 2.
 * The BKER bit in FLASH_CR selects Bank 2; PNB holds the page index
 * within the bank (0-255), so page 511 overall = bank 2, page 255.
 * --------------------------------------------------------------------- */
#define CONFIG_FLASH_ADDR   0x080FF800UL
#define CONFIG_FLASH_BANK   FLASH_BANK_2
#define CONFIG_FLASH_PAGE   255U

/* -----------------------------------------------------------------------
 * Internal RAM copy of the config.
 * All public functions read from and write to this variable.
 * Flash is only touched during Init and ApplyParam.
 * --------------------------------------------------------------------- */
static ConfigData_t s_config;

/* -----------------------------------------------------------------------
 * Flash storage layer — the only two functions that touch hardware.
 * To move config storage to a different page or a different medium,
 * only these two functions need to change.
 * --------------------------------------------------------------------- */

/*
 * Copies the ConfigData_t stored in Flash into *out.
 * This is a plain memory read — no HAL call needed, the Flash address
 * is directly accessible in the CPU memory map.
 */
static void config_flash_read(ConfigData_t *out)
{
    (void)memcpy(out, (const void *)CONFIG_FLASH_ADDR, sizeof(ConfigData_t));
}

/*
 * Erases Flash page 511 and writes *in to it as a sequence of
 * double-words (8 bytes each).  The sequence is:
 *   unlock -> erase page -> program double-words -> lock.
 * On any HAL error the function prints a diagnostic and returns early,
 * leaving the RAM copy intact so the system keeps running with its
 * last-known config.
 */
static void config_flash_write(const ConfigData_t *in)
{
    FLASH_EraseInitTypeDef erase;
    uint32_t               page_error;
    HAL_StatusTypeDef      status;
    const uint32_t        *src;
    uint32_t               addr;
    uint32_t               i;
    uint32_t               num_dwords;

    /* Step 1: unlock the Flash control register */
    status = HAL_FLASH_Unlock();
    if (HAL_OK != status)
    {
        printf("[CONFIG] Flash unlock failed\r\n");
        return;
    }

    /* Step 2: erase page 511 (2 KB) before writing.
     * Flash cells can only be programmed from 1 -> 0; erasing sets
     * them all back to 1 (0xFF).  Without an erase, writing a
     * previously-written location corrupts the data. */
    erase.TypeErase = FLASH_TYPEERASE_PAGES;
    erase.Banks     = CONFIG_FLASH_BANK;
    erase.Page      = CONFIG_FLASH_PAGE;
    erase.NbPages   = 1U;

    status = HAL_FLASHEx_Erase(&erase, &page_error);
    if (HAL_OK != status)
    {
        printf("[CONFIG] Flash erase failed (page_error=0x%08lX)\r\n",
               (unsigned long)page_error);
        HAL_FLASH_Lock();
        return;
    }

    /* Step 3: write the struct as double-words (64 bits = 8 bytes each).
     * The L476 Flash controller requires exactly two 32-bit words per
     * program operation — a 64-bit double-word.  We cast the struct
     * pointer to uint32_t* and step through it two words at a time.
     * num_dwords = total bytes / 8, rounded up to be safe. */
    src       = (const uint32_t *)in;
    addr      = CONFIG_FLASH_ADDR;
    num_dwords = (sizeof(ConfigData_t) + 7U) / 8U;

    for (i = 0U; i < num_dwords; ++i)
    {
        /* HAL_FLASH_Program with FLASH_TYPEPROGRAM_DOUBLEWORD takes the
         * lower 32-bit word first (address), then the upper 32-bit word
         * is read from address+4 automatically.  We pass the two words
         * packed into a uint64_t. */
        uint64_t dword;
        uint32_t lo;
        uint32_t hi;

        lo = src[2U * i];

        /* Guard: if the struct is not a multiple of 8 bytes the last
         * double-word may have only one real word.  Pad with 0xFFFFFFFF
         * (erased Flash value) so the ECC calculation stays clean. */
        if ((2U * i + 1U) * 4U < sizeof(ConfigData_t))
        {
            hi = src[2U * i + 1U];
        }
        else
        {
            hi = 0xFFFFFFFFUL;
        }

        dword  = (uint64_t)hi << 32U;
        dword |= (uint64_t)lo;

        status = HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD,
                                   addr,
                                   dword);
        if (HAL_OK != status)
        {
            printf("[CONFIG] Flash program failed at 0x%08lX\r\n",
                   (unsigned long)addr);
            HAL_FLASH_Lock();
            return;
        }

        addr += 8U;
    }

    /* Step 4: lock the Flash control register again.
     * Leaving it unlocked would allow accidental writes. */
    HAL_FLASH_Lock();
}

/* -----------------------------------------------------------------------
 * Internal helpers
 * --------------------------------------------------------------------- */

/*
 * Fills *cfg with the compile-time default values.
 * Called when Flash is blank or corrupt.
 */
static void config_load_defaults(ConfigData_t *cfg)
{
    cfg->magic            = CONFIG_MAGIC;
    cfg->temp_normal_low  = CONFIG_DEFAULT_TEMP_NORMAL_LOW;
    cfg->temp_normal_high = CONFIG_DEFAULT_TEMP_NORMAL_HIGH;
    cfg->temp_warning_low = CONFIG_DEFAULT_TEMP_WARNING_LOW;
    cfg->temp_warning_high= CONFIG_DEFAULT_TEMP_WARNING_HIGH;
    cfg->hum_normal_min   = CONFIG_DEFAULT_HUM_NORMAL_MIN;
    cfg->hum_warning_min  = CONFIG_DEFAULT_HUM_WARNING_MIN;
    cfg->light_normal_min = CONFIG_DEFAULT_LIGHT_NORMAL_MIN;
    cfg->light_warning_min= CONFIG_DEFAULT_LIGHT_WARNING_MIN;
    cfg->batt_normal_min  = CONFIG_DEFAULT_BATT_NORMAL_MIN;
    cfg->batt_warning_min = CONFIG_DEFAULT_BATT_WARNING_MIN;
}

/*
 * Posts a CONFIG_CHANGED event to the Comm TX queue.
 * Called after every successful Flash write so the Central Computer
 * is notified that a config parameter changed.
 * param_id is passed as the event detail field per the protocol spec.
 */
static void config_notify(uint8_t param_id)
{
    CommMsg_t msg;

    msg.type                    = COMM_MSG_EVENT;
    msg.payload.event.event_type = COMM_EVENT_CONFIG_CHANGED;
    msg.payload.event.detail    = param_id;
    msg.payload.event.timestamp = HAL_GetTick() / 1000U;

    (void)osMessageQueuePut(Comm_GetTxQueueHandle(),
                            &msg,
                            0U,
                            0U);
}

/* -----------------------------------------------------------------------
 * Public functions
 * --------------------------------------------------------------------- */

void Config_Init(void)
{
    /* Read whatever is currently in Flash page 511 into the RAM copy */
    config_flash_read(&s_config);

    /* Check the magic number.  If it does not match, Flash has never
     * been written by this firmware (first boot) or the page is corrupt.
     * Load defaults, write them to Flash, and notify. */
    if (CONFIG_MAGIC != s_config.magic)
    {
        printf("[CONFIG] Flash blank or corrupt — loading defaults\r\n");
        config_load_defaults(&s_config);
        config_flash_write(&s_config);
        config_notify(0U); /* param_id 0 = startup default load */
    }
    else
    {
        printf("[CONFIG] Loaded config from Flash\r\n");
    }
}

ConfigData_t Config_Get(void)
{
    /* Return a copy of the RAM shadow.
     * No Flash read needed — s_config is always in sync. */
    return s_config;
}

void Config_ApplyParam(uint8_t param_id,
                       const uint8_t *payload,
                       uint8_t payload_len)
{
    /* Each case reads the payload bytes into the correct field(s).
     * Multi-byte values are little-endian on the wire per the protocol
     * spec, so we reconstruct them byte by byte rather than casting
     * directly — avoids alignment and endianness assumptions. */

    switch (param_id)
    {
        case CONFIG_PARAM_TEMP_NORMAL:
            /* payload: low int16 (2 B) + high int16 (2 B) */
            if (payload_len < 4U)
            {
                printf("[CONFIG] TEMP_NORMAL: short payload\r\n");
                return;
            }
            s_config.temp_normal_low  = (int16_t)
                ((uint16_t)payload[0] | ((uint16_t)payload[1] << 8U));
            s_config.temp_normal_high = (int16_t)
                ((uint16_t)payload[2] | ((uint16_t)payload[3] << 8U));
            break;

        case CONFIG_PARAM_TEMP_WARNING:
            /* payload: low int16 (2 B) + high int16 (2 B) */
            if (payload_len < 4U)
            {
                printf("[CONFIG] TEMP_WARNING: short payload\r\n");
                return;
            }
            s_config.temp_warning_low  = (int16_t)
                ((uint16_t)payload[0] | ((uint16_t)payload[1] << 8U));
            s_config.temp_warning_high = (int16_t)
                ((uint16_t)payload[2] | ((uint16_t)payload[3] << 8U));
            break;

        case CONFIG_PARAM_HUM_NORMAL_MIN:
            /* payload: uint8 (1 B) */
            if (payload_len < 1U)
            {
                printf("[CONFIG] HUM_NORMAL_MIN: short payload\r\n");
                return;
            }
            s_config.hum_normal_min = payload[0];
            break;

        case CONFIG_PARAM_HUM_WARNING_MIN:
            /* payload: uint8 (1 B) */
            if (payload_len < 1U)
            {
                printf("[CONFIG] HUM_WARNING_MIN: short payload\r\n");
                return;
            }
            s_config.hum_warning_min = payload[0];
            break;

        case CONFIG_PARAM_LIGHT_NORMAL_MIN:
            /* payload: uint16 (2 B) little-endian */
            if (payload_len < 2U)
            {
                printf("[CONFIG] LIGHT_NORMAL_MIN: short payload\r\n");
                return;
            }
            s_config.light_normal_min =
                (uint16_t)payload[0] | ((uint16_t)payload[1] << 8U);
            break;

        case CONFIG_PARAM_LIGHT_WARNING_MIN:
            /* payload: uint16 (2 B) little-endian */
            if (payload_len < 2U)
            {
                printf("[CONFIG] LIGHT_WARNING_MIN: short payload\r\n");
                return;
            }
            s_config.light_warning_min =
                (uint16_t)payload[0] | ((uint16_t)payload[1] << 8U);
            break;

        case CONFIG_PARAM_BATT_NORMAL_MIN:
            /* payload: uint16 (2 B) little-endian */
            if (payload_len < 2U)
            {
                printf("[CONFIG] BATT_NORMAL_MIN: short payload\r\n");
                return;
            }
            s_config.batt_normal_min =
                (uint16_t)payload[0] | ((uint16_t)payload[1] << 8U);
            break;

        case CONFIG_PARAM_BATT_WARNING_MIN:
            /* payload: uint16 (2 B) little-endian */
            if (payload_len < 2U)
            {
                printf("[CONFIG] BATT_WARNING_MIN: short payload\r\n");
                return;
            }
            s_config.batt_warning_min =
                (uint16_t)payload[0] | ((uint16_t)payload[1] << 8U);
            break;

        default:
            printf("[CONFIG] Unknown param_id 0x%02X — ignored\r\n",
                   (unsigned)param_id);
            return;
    }

    /* Write the updated RAM copy back to Flash and notify */
    config_flash_write(&s_config);
    config_notify(param_id);

    printf("[CONFIG] param_id 0x%02X applied and saved\r\n",
           (unsigned)param_id);
}

void Config_RestoreForTest(const ConfigData_t *cfg)
{
    /* Overwrite the RAM shadow directly, then persist to Flash.
     * Bypasses Config_ApplyParam deliberately — we are restoring a
     * known-good snapshot, not processing a wire command. */
    s_config = *cfg;
    config_flash_write(&s_config);
}