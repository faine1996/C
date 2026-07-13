#include "sd_test.h"
#include "fatfs.h"
#include "main.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;
extern SPI_HandleTypeDef hspi1;

#define SD_TEST_PRINTF_BUF_SIZE 128U
#define SD_TEST_READ_BUF_SIZE 64U

static void SD_Test_Printf(const char *format, ...)
{
    static char buffer[SD_TEST_PRINTF_BUF_SIZE];
    va_list args;
    int length;

    va_start(args, format);
    length = vsnprintf(buffer, SD_TEST_PRINTF_BUF_SIZE, format, args);
    va_end(args);

    if (0 < length)
    {
        HAL_UART_Transmit(&huart2, (uint8_t *)buffer, (uint16_t)length,
                          HAL_MAX_DELAY);
    }
}

static void SD_Test_Halt(FRESULT error_code)
{
    SD_Test_Printf("SD test error: %d\r\n", (int)error_code);

    while (1)
    {
    }
}

static uint8_t SD_Test_Xchg(uint8_t data)
{
    uint8_t rx;

    HAL_SPI_TransmitReceive(&SD_SPI_HANDLE, &data, &rx, 1U, 50U);
    return rx;
}

static void SD_Test_Deselect(void)
{
    HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
    SD_Test_Xchg(0xFFU);
}

static uint8_t SD_Test_Select(void)
{
    uint8_t i;
    uint8_t response;

    HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_RESET);
    SD_Test_Xchg(0xFFU);

    for (i = 0U; i < 10U; i++)
    {
        response = SD_Test_Xchg(0xFFU);
        if (0xFFU == response)
        {
            return 1U;
        }
    }

    SD_Test_Deselect();
    return 0U;
}

static uint8_t SD_Test_SendCmd(uint8_t cmd, uint32_t arg, uint8_t crc)
{
    uint8_t n;
    uint8_t response;

    SD_Test_Deselect();
    if (0U == SD_Test_Select())
    {
        return 0xFFU;
    }

    SD_Test_Xchg((uint8_t)(0x40U | cmd));
    SD_Test_Xchg((uint8_t)(arg >> 24));
    SD_Test_Xchg((uint8_t)(arg >> 16));
    SD_Test_Xchg((uint8_t)(arg >> 8));
    SD_Test_Xchg((uint8_t)arg);
    SD_Test_Xchg(crc);

    n = 10U;
    do
    {
        response = SD_Test_Xchg(0xFFU);
        n--;
    } while ((0U != (response & 0x80U)) && (0U != n));

    return response;
}

void SD_Test_ProbeInit(void)
{
    uint8_t response;
    uint8_t trailing[4];
    uint8_t i;
    uint32_t start_tick;
    uint32_t iterations;

    HAL_Delay(1000);

    HAL_GPIO_WritePin(SD_CS_GPIO_Port, SD_CS_Pin, GPIO_PIN_SET);
    for (i = 0U; i < 10U; i++)
    {
        SD_Test_Xchg(0xFFU);
    }

    response = SD_Test_SendCmd(0U, 0U, 0x95U);
    SD_Test_Printf("CMD0 response: 0x%02X\r\n", response);

    if (0x01U != response)
    {
        SD_Test_Printf("Card did not enter idle state, stopping\r\n");
        SD_Test_Deselect();
        return;
    }

    response = SD_Test_SendCmd(8U, 0x1AAUL, 0x87U);
    SD_Test_Printf("CMD8 response: 0x%02X\r\n", response);

    for (i = 0U; i < 4U; i++)
    {
        trailing[i] = SD_Test_Xchg(0xFFU);
    }
    SD_Test_Printf("CMD8 trailing bytes: 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
                   trailing[0], trailing[1], trailing[2], trailing[3]);

    start_tick = HAL_GetTick();
    iterations = 0U;

    if (0U == (response & 0x04U))
    {
        SD_Test_Printf("Card reports SDv2, running ACMD41 (HCS) loop\r\n");
        do
        {
            SD_Test_SendCmd(55U, 0U, 0x01U);
            response = SD_Test_SendCmd(41U, 0x40000000UL, 0x01U);
            iterations++;
        } while ((0x00U != response) && ((HAL_GetTick() - start_tick) < 1000U));
    }
    else
    {
        SD_Test_Printf("CMD8 illegal, card reports SDv1/MMC, running ACMD41 "
                       "(no HCS) loop\r\n");
        do
        {
            SD_Test_SendCmd(55U, 0U, 0x01U);
            response = SD_Test_SendCmd(41U, 0U, 0x01U);
            iterations++;
        } while ((0x00U != response) && ((HAL_GetTick() - start_tick) < 1000U));
    }

    SD_Test_Printf("ACMD41 final response: 0x%02X after %lu iterations\r\n",
                   response, iterations);

    if (0x00U == response)
    {
        response = SD_Test_SendCmd(58U, 0U, 0x01U);
        SD_Test_Printf("CMD58 response: 0x%02X\r\n", response);

        for (i = 0U; i < 4U; i++)
        {
            trailing[i] = SD_Test_Xchg(0xFFU);
        }
        SD_Test_Printf("OCR bytes: 0x%02X 0x%02X 0x%02X 0x%02X\r\n",
                       trailing[0], trailing[1], trailing[2], trailing[3]);
        SD_Test_Printf("Card initialization succeeded\r\n");
    }
    else
    {
        SD_Test_Printf("Card never left busy state, initialization failed\r\n");
    }

    SD_Test_Deselect();
}

void SD_Test_Run(void)
{
    FRESULT fr;
    FATFS fs;
    FIL file;
    char read_buffer[SD_TEST_READ_BUF_SIZE];
    const char write_data[] = "Hello, world!\r\n";
    UINT bytes_written;

    HAL_Delay(1000);

    fr = f_mount(&fs, "", 1);
    if (FR_OK != fr)
    {
        SD_Test_Halt(fr);
    }

    fr = f_open(&file, "hello.txt",
                FA_WRITE | FA_OPEN_ALWAYS | FA_CREATE_ALWAYS);
    if (FR_OK != fr)
    {
        SD_Test_Halt(fr);
    }

    fr = f_write(&file, write_data, strlen(write_data), &bytes_written);
    if (FR_OK != fr)
    {
        SD_Test_Halt(fr);
    }

    SD_Test_Printf("Wrote %u bytes to hello.txt\r\n", bytes_written);

    f_close(&file);

    fr = f_open(&file, "hello.txt", FA_READ);
    if (FR_OK != fr)
    {
        SD_Test_Halt(fr);
    }

    if (0 != f_gets(read_buffer, sizeof(read_buffer), &file))
    {
        SD_Test_Printf("hello.txt contents: %s", read_buffer);
    }

    f_close(&file);

    f_mount(NULL, "", 0);
}