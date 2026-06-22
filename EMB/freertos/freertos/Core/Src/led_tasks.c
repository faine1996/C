#include "led_tasks.h"
#include <stdio.h>

extern UART_HandleTypeDef huart2;

volatile unsigned long led1_counter = 0;
volatile unsigned long led2_counter = 0;

void Task_LED1(void *argument)
{
    (void)argument;
    for (;;)
    {
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
        led1_counter++;
        osDelay(300);
    }
}

void Task_LED2(void *argument)
{
    (void)argument;
    for (;;)
    {
        HAL_GPIO_TogglePin(LED2_PORT, LED2_PIN);
        led2_counter++;
        osDelay(500);
    }
}

void Task_Reporter(void *argument)
{
    char buf[64];
    int len;

    (void)argument;
    for (;;)
    {
        len = sprintf(buf, "LED1: %lu  LED2: %lu\r\n", led1_counter,
                      led2_counter);
        HAL_UART_Transmit(&huart2, (uint8_t *)buf, (uint16_t)len,
                          HAL_MAX_DELAY);
        osDelay(3000);
    }
}