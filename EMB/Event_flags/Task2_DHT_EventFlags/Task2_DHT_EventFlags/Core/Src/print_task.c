#include "print_task.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "dht_task.h"
#include "main.h"
#include <stdio.h>

extern UART_HandleTypeDef huart2;

void PrintTask(void *argument)
{
    char buf[48];
    int len;
    (void)argument;
    for (;;)
    {
        osEventFlagsWait(xEventGroup, FLAG_PRINT, osFlagsWaitAny,
                         osWaitForever);
        len = sprintf(buf, "Temp: %u C  Hum: %u %%\r\n",
                      (unsigned int)g_temperature, (unsigned int)g_humidity);
        HAL_UART_Transmit(&huart2, (uint8_t *)buf, (uint16_t)len,
                          HAL_MAX_DELAY);
    }
}