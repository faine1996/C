#include "dht11.h"
#include "main.h"
#include "test_bench.h"
#include <stdio.h>
#include <string.h>

static uint8_t DwtInitialized = 0U;

static void Dht11_DelayUs(uint32_t us)
{
    uint32_t start;
    uint32_t ticks;

    if (0U == DwtInitialized)
    {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
        DwtInitialized = 1U;
    }

    start = DWT->CYCCNT;
    ticks = us * (SystemCoreClock / 1000000U);

    while (ticks > (DWT->CYCCNT - start))
    {
    }
}

static void Dht11_SetPinOutput(void)
{
    GPIO_InitTypeDef gpio_init;

    gpio_init.Pin = DHT_DATA_Pin;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(DHT_DATA_GPIO_Port, &gpio_init);
}

static void Dht11_SetPinInput(void)
{
    GPIO_InitTypeDef gpio_init;

    gpio_init.Pin = DHT_DATA_Pin;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(DHT_DATA_GPIO_Port, &gpio_init);
}

static uint8_t Dht11_WaitForLevel(GPIO_PinState level, uint32_t timeout_us)
{
    uint32_t start;
    uint32_t elapsed_us;

    start = DWT->CYCCNT;

    while (level != HAL_GPIO_ReadPin(DHT_DATA_GPIO_Port, DHT_DATA_Pin))
    {
        elapsed_us = (DWT->CYCCNT - start) / (SystemCoreClock / 1000000U);

        if (timeout_us < elapsed_us)
        {
            return (0U);
        }
    }

    return (1U);
}

void Dht11_Init(void)
{
    Dht11_DelayUs(1U);
    Dht11_SetPinInput();
}

Dht11Status_t Dht11_Read(int8_t *temperature, uint8_t *humidity)
{
    uint8_t data[5];
    uint8_t byte_idx;
    uint8_t bit_idx;
    uint8_t checksum;
    uint32_t high_us;
    uint32_t start;

    (void)memset(data, 0, sizeof(data));

    Dht11_SetPinOutput();
    HAL_GPIO_WritePin(DHT_DATA_GPIO_Port, DHT_DATA_Pin, GPIO_PIN_RESET);
    HAL_Delay(20U);
    HAL_GPIO_WritePin(DHT_DATA_GPIO_Port, DHT_DATA_Pin, GPIO_PIN_SET);
    Dht11_DelayUs(30U);
    Dht11_SetPinInput();

    if (0U == Dht11_WaitForLevel(GPIO_PIN_RESET, 100U))
    {
        return (DHT11_ERROR_TIMEOUT);
    }

    if (0U == Dht11_WaitForLevel(GPIO_PIN_SET, 100U))
    {
        return (DHT11_ERROR_TIMEOUT);
    }

    if (0U == Dht11_WaitForLevel(GPIO_PIN_RESET, 100U))
    {
        return (DHT11_ERROR_TIMEOUT);
    }

    for (byte_idx = 0U; byte_idx < 5U; ++byte_idx)
    {
        for (bit_idx = 0U; bit_idx < 8U; ++bit_idx)
        {
            if (0U == Dht11_WaitForLevel(GPIO_PIN_SET, 100U))
            {
                return (DHT11_ERROR_TIMEOUT);
            }

            start = DWT->CYCCNT;

            if (0U == Dht11_WaitForLevel(GPIO_PIN_RESET, 100U))
            {
                return (DHT11_ERROR_TIMEOUT);
            }

            high_us = (DWT->CYCCNT - start) / (SystemCoreClock / 1000000U);

            data[byte_idx] <<= 1;

            if (40U < high_us)
            {
                data[byte_idx] |= 1U;
            }
        }
    }

    checksum = (uint8_t)(data[0] + data[1] + data[2] + data[3]);

    if (checksum != data[4])
    {
        return (DHT11_ERROR_CHECKSUM);
    }

    *humidity = data[0];
    *temperature = (int8_t)data[2];

    return (DHT11_OK);
}

void Dht11_Test(void)
{
    int8_t temperature;
    uint8_t humidity;
    Dht11Status_t status;

    printf("DHT11 test: reading sensor...\r\n");

    status = Dht11_Read(&temperature, &humidity);

    if (DHT11_OK == status)
    {
        printf("Temperature: %d C, Humidity: %u %%\r\n",
               (int)temperature, (unsigned int)humidity);
        TEST_CHECK(1, "DHT11 read succeeded");
    }
    else
    {
        printf("DHT11 read failed, status = %d\r\n", (int)status);
        TEST_CHECK(0, "DHT11 read succeeded");
    }
}