#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "task_manager.h"

void Delay500Task(void *argument)
{
    (void)argument;
    for (;;)
    {
        osEventFlagsWait(xEventGroup, FLAG_START500, osFlagsWaitAny,
                         osWaitForever);
        osDelay(500);
        osEventFlagsSet(xEventGroup, FLAG_DONE500);
    }
}

void Delay700Task(void *argument)
{
    (void)argument;
    for (;;)
    {
        osEventFlagsWait(xEventGroup, FLAG_START700, osFlagsWaitAny,
                         osWaitForever);
        osDelay(700);
        osEventFlagsSet(xEventGroup, FLAG_DONE700);
    }
}

void Delay1100Task(void *argument)
{
    (void)argument;
    for (;;)
    {
        osEventFlagsWait(xEventGroup, FLAG_START1100, osFlagsWaitAny,
                         osWaitForever);
        osDelay(1100);
        osEventFlagsSet(xEventGroup, FLAG_DONE1100);
    }
}