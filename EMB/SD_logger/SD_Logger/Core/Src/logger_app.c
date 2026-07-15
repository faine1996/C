#include "logger_app.h"
#include "FreeRTOS.h"
#include "cmsis_os2.h"
#include "dht11.h"
#include "main.h"
#include "sd_log.h"
#include "sensor_types.h"
#include "task.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

#define DHT_QUEUE_LEN (4U)
#define DHT_TASK_STACK (512U)
#define SAVE_TASK_STACK (1024U)
#define DHT_PERIOD_MS (3000U)
#define MOUNT_RETRY_MS (1000U)

static osMessageQueueId_t DhtQueueHandle;

static void LoggerApp_Report(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, (uint16_t)strlen(msg),
                      HAL_MAX_DELAY);
}

static void DhtTask(void *argument)
{
    DhtReading_t reading;
    DHT11_Status_t status;
    uint32_t wake_tick;
    char report[64];

    (void)argument;

    DHT11_Init();

    wake_tick = osKernelGetTickCount();

    for (;;)
    {
        wake_tick += DHT_PERIOD_MS;
        osDelayUntil(wake_tick);

        status = DHT11_Read(&reading);

        if (DHT11_OK == status)
        {
            if (osOK != osMessageQueuePut(DhtQueueHandle, &reading, 0U, 0U))
            {
                LoggerApp_Report("DHT: queue full, reading dropped\r\n");
            }
        }
        else
        {
            sprintf(report, "DHT: read failed (%d)\r\n", (int)status);
            LoggerApp_Report(report);
        }
    }
}

static void SaveTask(void *argument)
{
    DhtReading_t reading;
    SdLog_Status_t status;

    (void)argument;

    while (SD_LOG_OK != SdLog_Mount())
    {
        LoggerApp_Report("SD: mount failed, retrying\r\n");
        osDelay(MOUNT_RETRY_MS);
    }

    LoggerApp_Report("SD: mounted\r\n");

    for (;;)
    {
        if (osOK ==
            osMessageQueueGet(DhtQueueHandle, &reading, NULL, osWaitForever))
        {
            status = SdLog_AppendReading(&reading);

            if (SD_LOG_OK == status)
            {
                HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
            }
            else
            {
                HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
                LoggerApp_Report("SD: write failed\r\n");
            }
        }
    }
}

void LoggerApp_Init(void)
{
    osThreadAttr_t dht_task_attr;
    osThreadAttr_t save_task_attr;

    memset(&dht_task_attr, 0, sizeof(dht_task_attr));
    dht_task_attr.name = "DhtTask";
    dht_task_attr.stack_size = DHT_TASK_STACK * sizeof(StackType_t);
    dht_task_attr.priority = osPriorityAboveNormal;

    memset(&save_task_attr, 0, sizeof(save_task_attr));
    save_task_attr.name = "SaveTask";
    save_task_attr.stack_size = SAVE_TASK_STACK * sizeof(StackType_t);
    save_task_attr.priority = osPriorityNormal;

    DhtQueueHandle =
        osMessageQueueNew(DHT_QUEUE_LEN, sizeof(DhtReading_t), NULL);

    (void)osThreadNew(DhtTask, NULL, &dht_task_attr);
    (void)osThreadNew(SaveTask, NULL, &save_task_attr);
}