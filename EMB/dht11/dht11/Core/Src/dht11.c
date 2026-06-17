#include "dht11.h"
#include "main.h"

#define DHT11_PORT  DHT11_DATA_GPIO_Port
#define DHT11_PIN   DHT11_DATA_Pin
#define TIMEOUT_US  200U

static TIM_HandleTypeDef *s_htim;

static void us_delay(uint32_t us);
static void pin_output(void);
static void pin_input(void);
static int read_bit(void);

void DHT11_Init(TIM_HandleTypeDef *htim)
{
    s_htim = htim;
    HAL_TIM_Base_Start(s_htim);
    pin_output();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
}

DHT11_Status_t DHT11_Read(DHT11_Data_t *out)
{
    uint8_t  bytes[5] = {0};
    uint32_t start;
    int      bit;
    int      i;
    int      b;

    pin_output();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    us_delay(1000);
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
    HAL_Delay(18);
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    us_delay(40);
    pin_input();

    start = __HAL_TIM_GET_COUNTER(s_htim);
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET) 
    {
        if (TIMEOUT_US < (__HAL_TIM_GET_COUNTER(s_htim) - start))
        {
            return DHT11_TIMEOUT;
        }
    }

    start = __HAL_TIM_GET_COUNTER(s_htim);
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET) 
    {
        if (TIMEOUT_US < (__HAL_TIM_GET_COUNTER(s_htim) - start))
        {
            return DHT11_TIMEOUT;
        }
    }

    for (i = 0; i < 5; i++) 
    {
        for (b = 7; b >= 0; b--) 
        {
            bit = read_bit();
            if (bit < 0)
            {
                return DHT11_TIMEOUT;
            }
            bytes[i] |= (uint8_t)(bit << b);
        }
    }

    if (((bytes[0] + bytes[1] + bytes[2] + bytes[3]) & 0xFF) != bytes[4])
    {
        return DHT11_CHECKSUM_ERR;
    }

    out->humidity_int = bytes[0];
    out->humidity_dec = bytes[1];
    out->temp_int     = bytes[2];
    out->temp_dec     = bytes[3];

    return DHT11_OK;
}

static void us_delay(uint32_t us)
{
    uint32_t start = __HAL_TIM_GET_COUNTER(s_htim);
    while ((__HAL_TIM_GET_COUNTER(s_htim) - start) < us);
}

static void pin_output(void)
{
    GPIO_InitTypeDef g = {0};
    g.Pin   = DHT11_PIN;
    g.Mode  = GPIO_MODE_OUTPUT_PP;
    g.Pull  = GPIO_NOPULL;
    g.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PORT, &g);
}

static void pin_input(void)
{
    GPIO_InitTypeDef g = {0};
    g.Pin  = DHT11_PIN;
    g.Mode = GPIO_MODE_INPUT;
    g.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_PORT, &g);
}

static int read_bit(void)
{
    uint32_t start, elapsed;

    start = __HAL_TIM_GET_COUNTER(s_htim);
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET) 
    {
        if (TIMEOUT_US < (__HAL_TIM_GET_COUNTER(s_htim) - start))
        {
            return -1;
        }
    }
        start = __HAL_TIM_GET_COUNTER(s_htim);
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET)
    {
        if (TIMEOUT_US < (__HAL_TIM_GET_COUNTER(s_htim) - start))
        {
            return -1;
        }
    }
    elapsed = __HAL_TIM_GET_COUNTER(s_htim) - start;

    return (elapsed > 40U) ? 1 : 0;
}