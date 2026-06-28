#include "dht11.h"
#include "FreeRTOS.h"
#include "task.h"

static void delay_us(uint32_t us)
{
    uint32_t start;
    uint32_t ticks;

    start = DWT->CYCCNT;
    ticks = us * (SystemCoreClock / 1000000U);
    while ((DWT->CYCCNT - start) < ticks)
    {
    }
}

static void set_pin_output(DHT11_t *dht)
{
    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = dht->pin;
    gpio_init.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull = GPIO_NOPULL;
    gpio_init.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(dht->port, &gpio_init);
}

static void set_pin_input(DHT11_t *dht)
{
    GPIO_InitTypeDef gpio_init = {0};
    gpio_init.Pin = dht->pin;
    gpio_init.Mode = GPIO_MODE_INPUT;
    gpio_init.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(dht->port, &gpio_init);
}

void DHT11_Init(DHT11_t *dht, GPIO_TypeDef *port, uint16_t pin)
{
    dht->port = port;
    dht->pin = pin;

    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    set_pin_output(dht);
    HAL_GPIO_WritePin(dht->port, dht->pin, GPIO_PIN_SET);
}

DHT11_Status_t DHT11_Read(DHT11_t *dht, uint8_t *temperature, uint8_t *humidity)
{
    uint8_t data[5];
    uint8_t i;
    uint8_t j;
    uint16_t timeout;

    data[0] = 0;
    data[1] = 0;
    data[2] = 0;
    data[3] = 0;
    data[4] = 0;

    /* Start signal — interrupts still on, HAL_Delay needs SysTick */
    set_pin_output(dht);
    HAL_GPIO_WritePin(dht->port, dht->pin, GPIO_PIN_RESET);
    HAL_Delay(20);
    HAL_GPIO_WritePin(dht->port, dht->pin, GPIO_PIN_SET);

    /* Enter critical section BEFORE the 30us release window —
       prevents scheduler from preempting and destroying DHT timing */
    taskENTER_CRITICAL();

    delay_us(30);
    set_pin_input(dht);

    /* Wait for DHT to pull low (start of response) */
    timeout = 0;
    while (GPIO_PIN_SET == HAL_GPIO_ReadPin(dht->port, dht->pin))
    {
        delay_us(1);
        if (100 < ++timeout)
        {
            taskEXIT_CRITICAL();
            return DHT11_ERROR;
        }
    }

    /* Wait for DHT to pull high (~80 us response pulse) */
    timeout = 0;
    while (GPIO_PIN_RESET == HAL_GPIO_ReadPin(dht->port, dht->pin))
    {
        delay_us(1);
        if (100 < ++timeout)
        {
            taskEXIT_CRITICAL();
            return DHT11_ERROR;
        }
    }

    /* Wait for DHT to pull low again (data transmission begins) */
    timeout = 0;
    while (GPIO_PIN_SET == HAL_GPIO_ReadPin(dht->port, dht->pin))
    {
        delay_us(1);
        if (100 < ++timeout)
        {
            taskEXIT_CRITICAL();
            return DHT11_ERROR;
        }
    }

    /* Read 40 bits = 5 bytes */
    for (i = 0; i < 5; i++)
    {
        data[i] = 0;
        for (j = 0; j < 8; j++)
        {
            timeout = 0;
            while (GPIO_PIN_RESET == HAL_GPIO_ReadPin(dht->port, dht->pin))
            {
                delay_us(1);
                if (100 < ++timeout)
                {
                    taskEXIT_CRITICAL();
                    return DHT11_ERROR;
                }
            }

            delay_us(40);
            data[i] <<= 1;
            if (GPIO_PIN_SET == HAL_GPIO_ReadPin(dht->port, dht->pin))
            {
                data[i] |= 0x01;
            }

            timeout = 0;
            while (GPIO_PIN_SET == HAL_GPIO_ReadPin(dht->port, dht->pin))
            {
                delay_us(1);
                if (100 < ++timeout)
                {
                    taskEXIT_CRITICAL();
                    return DHT11_ERROR;
                }
            }
        }
    }

    taskEXIT_CRITICAL();

    if (data[4] != (uint8_t)((data[0] + data[1] + data[2] + data[3]) & 0xFF))
    {
        return DHT11_ERROR;
    }

    *humidity = data[0];
    *temperature = data[2];
    return DHT11_OK;
}