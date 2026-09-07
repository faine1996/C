#ifndef CONFIG_H
#define CONFIG_H

#include <stdint.h>

/* -----------------------------------------------------------------------
 * Magic number written to Flash alongside config data.
 * On startup, if the value at the config Flash address does not match
 * this constant, the module treats Flash as blank and writes defaults.
 * Changing this value forces a one-time reset to defaults on next boot.
 * --------------------------------------------------------------------- */
#define CONFIG_MAGIC  0xDEADBEEFUL

/* -----------------------------------------------------------------------
 * Default limit values — used on first boot or after a magic mismatch.
 * These match the hardcoded constants previously in monitor.c.
 * --------------------------------------------------------------------- */
#define CONFIG_DEFAULT_TEMP_NORMAL_LOW    ((int16_t)18)
#define CONFIG_DEFAULT_TEMP_NORMAL_HIGH   ((int16_t)28)
#define CONFIG_DEFAULT_TEMP_WARNING_LOW   ((int16_t)10)
#define CONFIG_DEFAULT_TEMP_WARNING_HIGH  ((int16_t)35)
#define CONFIG_DEFAULT_HUM_NORMAL_MIN     ((uint8_t)30U)
#define CONFIG_DEFAULT_HUM_WARNING_MIN    ((uint8_t)20U)
#define CONFIG_DEFAULT_LIGHT_NORMAL_MIN   ((uint16_t)700U)
#define CONFIG_DEFAULT_LIGHT_WARNING_MIN  ((uint16_t)300U)
#define CONFIG_DEFAULT_BATT_NORMAL_MIN    ((uint16_t)3000U)
#define CONFIG_DEFAULT_BATT_WARNING_MIN   ((uint16_t)1500U)

/* -----------------------------------------------------------------------
 * SET_CONFIG param IDs (from wire protocol spec).
 * These are the values that arrive in the first byte of a SET_CONFIG
 * frame's value field.  Config_ApplyParam switches on these.
 * --------------------------------------------------------------------- */
#define CONFIG_PARAM_TEMP_NORMAL      0x01U
#define CONFIG_PARAM_TEMP_WARNING     0x02U
#define CONFIG_PARAM_HUM_NORMAL_MIN   0x03U
#define CONFIG_PARAM_HUM_WARNING_MIN  0x04U
#define CONFIG_PARAM_LIGHT_NORMAL_MIN 0x05U
#define CONFIG_PARAM_LIGHT_WARNING_MIN 0x06U
#define CONFIG_PARAM_BATT_NORMAL_MIN  0x07U
#define CONFIG_PARAM_BATT_WARNING_MIN 0x08U

/* -----------------------------------------------------------------------
 * ConfigData_t — the single struct that holds all configurable limits.
 * This is what gets written to and read from Flash page 511.
 * Total size: 24 bytes (3 double-words, Flash-write aligned).
 * --------------------------------------------------------------------- */
typedef struct
{
    uint32_t magic;              /* CONFIG_MAGIC when valid              */

    int16_t  temp_normal_low;    /* degrees C — lower bound of Normal    */
    int16_t  temp_normal_high;   /* degrees C — upper bound of Normal    */
    int16_t  temp_warning_low;   /* degrees C — lower bound of Warning   */
    int16_t  temp_warning_high;  /* degrees C — upper bound of Warning   */

    uint8_t  hum_normal_min;     /* percent   — floor of Normal humidity */
    uint8_t  hum_warning_min;    /* percent   — floor of Warning humidity*/

    uint16_t light_normal_min;   /* ADC counts — floor of Normal light   */
    uint16_t light_warning_min;  /* ADC counts — floor of Warning light  */

    uint16_t batt_normal_min;    /* ADC counts — floor of Normal battery */
    uint16_t batt_warning_min;   /* ADC counts — floor of Warning battery*/
} ConfigData_t;                  /* 24 bytes total                       */

/* -----------------------------------------------------------------------
 * Public interface
 * --------------------------------------------------------------------- */

/**
 * @brief   Initialises the Config module. Reads ConfigData_t from Flash
 *          page 511. If the magic number is wrong (first boot or corrupt),
 *          writes default values to Flash and fires a CONFIG_CHANGED event.
 *          Must be called before Config_Get() or Config_ApplyParam().
 * @param   None.
 * @retval  None.
 */
void Config_Init(void);

/**
 * @brief   Returns a copy of the current configuration from RAM.
 *          Does not read Flash. Safe to call from any task.
 * @param   None.
 * @retval  ConfigData_t struct with all current limit values.
 */
ConfigData_t Config_Get(void);

/**
 * @brief   Applies a single parameter update received in a SET_CONFIG
 *          frame. Updates the RAM copy, writes the full struct to Flash,
 *          and posts a CONFIG_CHANGED event to the Comm TX queue.
 * @param   param_id     One of the CONFIG_PARAM_* constants above.
 * @param   payload      Pointer to the payload bytes following param_id
 *                       in the SET_CONFIG frame value field.
 * @param   payload_len  Number of payload bytes (caller's responsibility
 *                       to pass the correct count per the protocol spec).
 * @retval  None.
 */
void Config_ApplyParam(uint8_t param_id,
                       const uint8_t *payload,
                       uint8_t payload_len);

/**
 * @brief   Runs the Config self-test. Applies known values via
 *          Config_ApplyParam, reads back with Config_Get, verifies
 *          each field, then restores the original config.
 *          Reports pass/fail via TEST_CHECK.
 * @param   None.
 * @retval  None.
 */
void Config_Test(void);

/**
 * @brief   Restores a previously saved ConfigData_t directly to RAM and
 *          Flash. Exists solely for use by the self-test after it has
 *          modified live config values. Must not be called from
 *          application code.
 * @param   cfg  Pointer to the ConfigData_t to restore.
 * @retval  None.
 */
void Config_RestoreForTest(const ConfigData_t *cfg);

#endif /* CONFIG_H */