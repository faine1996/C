#include "dht11.h"
#include "main.h"
#include <string.h>

static void DWT_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
}

static void DWT_DelayUs(uint32_t us)
{
    uint32_t start;
    uint32_t ticks;

    start = DWT->CYCCNT;
    ticks = us * (SystemCoreClock / 1000000U);

    while (ticks > (DWT->CYCCNT - start))
    {
        /* busy wait */
    }
}

static void DHT11_SetPinOutput(void)
{
    GPIO_InitTypeDef gpio_init;

    gpio_init.Pin = DHT_PIN_Pin;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(DHT_PIN_GPIO_Port, &gpio_init);
}

static void DHT11_SetPinInput(void)
{
    GPIO_InitTypeDef gpio_init;

    gpio_init.Pin = DHT_PIN_Pin;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_FREQ_HIGH;

    HAL_GPIO_Init(DHT_PIN_GPIO_Port, &gpio_init);
}

static uint8_t DHT11_WaitForLevel(GPIO_PinState level, uint32_t timeout_us)
{
    uint32_t start;
    uint32_t elapsed_us;

    start = DWT->CYCCNT;

    while (level != HAL_GPIO_ReadPin(DHT_PIN_GPIO_Port, DHT_PIN_Pin))
    {
        elapsed_us = (DWT->CYCCNT - start) / (SystemCoreClock / 1000000U);

        if (timeout_us < elapsed_us)
        {
            return (0);
        }
    }

    return (1);
}

void DHT11_Init(void)
{
    DWT_Init();
    DHT11_SetPinInput();
}

DHT11_Status_t DHT11_Read(DhtReading_t *reading)
{
    uint8_t data[5];
    uint8_t byte_idx;
    uint8_t bit_idx;
    uint8_t checksum;
    uint32_t high_us;
    uint32_t start;

    memset(data, 0, sizeof(data));

    /* host start signal: pull low >= 18 ms, then release and let the line float
     * high */
    DHT11_SetPinOutput();
    HAL_GPIO_WritePin(DHT_PIN_GPIO_Port, DHT_PIN_Pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(DHT_PIN_GPIO_Port, DHT_PIN_Pin, GPIO_PIN_SET);
    DWT_DelayUs(30);
    DHT11_SetPinInput();

    /* sensor response preamble: 80us low, then 80us high */
    if (0 == DHT11_WaitForLevel(GPIO_PIN_RESET, 100))
    {
        return (DHT11_ERROR_TIMEOUT);
    }

    if (0 == DHT11_WaitForLevel(GPIO_PIN_SET, 100))
    {
        return (DHT11_ERROR_TIMEOUT);
    }

    if (0 == DHT11_WaitForLevel(GPIO_PIN_RESET, 100))
    {
        return (DHT11_ERROR_TIMEOUT);
    }

    /* 40 data bits: humidity int, humidity dec, temp int, temp dec, checksum */
    for (byte_idx = 0; byte_idx < 5; byte_idx++)
    {
        for (bit_idx = 0; bit_idx < 8; bit_idx++)
        {
            /* every bit starts with a fixed ~50us low pulse */
            if (0 == DHT11_WaitForLevel(GPIO_PIN_SET, 100))
            {
                return (DHT11_ERROR_TIMEOUT);
            }

            start = DWT->CYCCNT;

            /* bit value is encoded in the following high pulse length: ~26-28us
             * = 0, ~70us = 1 */
            if (0 == DHT11_WaitForLevel(GPIO_PIN_RESET, 100))
            {
                return (DHT11_ERROR_TIMEOUT);
            }

            high_us = (DWT->CYCCNT - start) / (SystemCoreClock / 1000000U);

            data[byte_idx] <<= 1;

            if (40 < high_us)
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

    reading->humidity = data[0];
    reading->temperature = data[2];
    reading->tick = HAL_GetTick();

    return (DHT11_OK);
}