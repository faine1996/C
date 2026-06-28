#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "main.h"
#include "task.h"
#include "task_manager.h"
#include <stdio.h>

extern UART_HandleTypeDef huart2;

void PrintTask(void *argument)
{
    char buf[48];
    int len;
    (void)argument;
    for (;;)
    {
        osEventFlagsWait(xEventGroup, FLAG_ALL_DONE, osFlagsWaitAll,
                         osWaitForever);
        len = sprintf(buf, "SysTick: %lu ms\r\n",
                      (unsigned long)xTaskGetTickCount());
        HAL_UART_Transmit(&huart2, (uint8_t *)buf, (uint16_t)len,
                          HAL_MAX_DELAY);
        osEventFlagsSet(xEventGroup, FLAG_ALL_START);
    }
}