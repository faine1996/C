#include "PrintTasks.h"
#include "cmsis_os.h"
#include "main.h"
#include <stdlib.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

osMutexId_t print_mutex;

static const char *MSG[3] = {
    "Task 1 **********************************************************\r\n",
    "Task 2 ----------------------------------------------------------\r\n",
    "Task 3 //////////////////////////////////////////////////////////\r\n"};

void PrintTask(void *argument)
{
    uintptr_t id;
    const char *msg;

    id = (uintptr_t)argument;
    msg = MSG[id - 1];

    for (;;)
    {
        osMutexAcquire(print_mutex, osWaitForever);
        HAL_UART_Transmit(&huart2, (uint8_t *)msg, (uint16_t)strlen(msg),
                          HAL_MAX_DELAY);
        osMutexRelease(print_mutex);
        osDelay(10 + (uint32_t)(rand() % 90));
    }
}

void PrintTasks_Init(void)
{
    print_mutex = osMutexNew(NULL);
}