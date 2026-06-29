#include "print_task.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "dht_task.h"
#include "main.h"
#include "task.h"
#include <stdio.h>

extern UART_HandleTypeDef huart2;

void PrintTask(void *argument)
{
    uint32_t value;
    uint8_t temp;
    uint8_t hum;
    char buf[48];
    int len;
    (void)argument;

    for (;;)
    {
        xTaskNotifyWait(0, 0xFFFFFFFF, &value, portMAX_DELAY);

        temp = (uint8_t)((value >> 16) & 0xFF);
        hum = (uint8_t)(value & 0xFF);

        len = sprintf(buf, "Temp: %u C  Hum: %u %%\r\n", (unsigned int)temp,
                      (unsigned int)hum);
        HAL_UART_Transmit(&huart2, (uint8_t *)buf, (uint16_t)len,
                          HAL_MAX_DELAY);
    }
}