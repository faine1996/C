#include "led_tasks.h"
#include <stdio.h>

extern UART_HandleTypeDef huart2;

LedTaskParams led1_params = {LED1_PORT, LED1_PIN, 300, 0};
LedTaskParams led2_params = {LED2_PORT, LED2_PIN, 500, 0};

void Task_LED(void *argument)
{
    LedTaskParams *params;

    params = (LedTaskParams *)argument;

    for (;;)
    {
        HAL_GPIO_TogglePin(params->port, params->pin);
        params->counter++;
        osDelay(params->delay_ms);
    }
}

void Task_Reporter(void *argument)
{
    char buf[64];
    int len;

    (void)argument;
    for (;;)
    {
        len = sprintf(buf, "LED1: %lu  LED2: %lu\r\n", led1_params.counter,
                      led2_params.counter);
        HAL_UART_Transmit(&huart2, (uint8_t *)buf, (uint16_t)len,
                          HAL_MAX_DELAY);
        osDelay(3000);
    }
}
