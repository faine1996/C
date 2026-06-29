#include "delay_task.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "dht_task.h"
#include "task.h"

void DelayTask(void *argument)
{
    (void)argument;
    for (;;)
    {
        osDelay(2000);
        xTaskNotifyGive(dhtTaskHandle);
    }
}