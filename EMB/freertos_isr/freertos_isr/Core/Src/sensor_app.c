#include "sensor_app.h"

#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "queue.h"
#include "task.h"

#include "dht11.h"
#include "main.h"

#include <stdio.h>
#include <string.h>

#define DHT11_GPIO_PORT GPIOB
#define DHT11_GPIO_PIN GPIO_PIN_5
#define SENSOR_QUEUE_DEPTH 8

extern TIM_HandleTypeDef htim7;
extern UART_HandleTypeDef huart2;

static TaskHandle_t dhtTaskHandle;
static QueueHandle_t sensorQueueHandle;
static DHT11_t dht11Sensor;

static void DhtTask(void *argument);
static void PrintTask(void *argument);

void SensorApp_Init(void)
{
    osThreadAttr_t dhtTaskAttr;
    osThreadAttr_t printTaskAttr;
    osMessageQueueId_t queueId;
    osThreadId_t dhtThreadId;

    DHT11_Init(&dht11Sensor, DHT11_GPIO_PORT, DHT11_GPIO_PIN);

    queueId = osMessageQueueNew((uint32_t)SENSOR_QUEUE_DEPTH, sizeof(SensorMsg),
                                NULL);
    sensorQueueHandle = (QueueHandle_t)queueId;

    memset(&dhtTaskAttr, 0, sizeof(dhtTaskAttr));
    dhtTaskAttr.name = "DhtTask";
    dhtTaskAttr.priority = (osPriority_t)osPriorityHigh;
    dhtTaskAttr.stack_size = 256 * 4;

    memset(&printTaskAttr, 0, sizeof(printTaskAttr));
    printTaskAttr.name = "PrintTask";
    printTaskAttr.priority = (osPriority_t)osPriorityNormal;
    printTaskAttr.stack_size = 256 * 4;

    dhtThreadId = osThreadNew(DhtTask, NULL, &dhtTaskAttr);
    dhtTaskHandle = (TaskHandle_t)dhtThreadId;

    (void)osThreadNew(PrintTask, NULL, &printTaskAttr);

    HAL_TIM_Base_Start_IT(&htim7);
}

static void DhtTask(void *argument)
{
    uint32_t notifiedBits;
    uint8_t temperature;
    uint8_t humidity;
    SensorMsg msg;

    (void)argument;

    for (;;)
    {
        notifiedBits = 0;
        xTaskNotifyWait(0xFFFFFFFFUL, 0xFFFFFFFFUL, &notifiedBits,
                        portMAX_DELAY);

        if (DHT11_OK == DHT11_Read(&dht11Sensor, &temperature, &humidity))
        {
            if (0UL != (notifiedBits & NOTIFY_BUTTON))
            {
                msg.id = MSG_ID_TEMPERATURE;
                msg.value = (int32_t)temperature;
                xQueueSend(sensorQueueHandle, &msg, 0);
            }

            if (0UL != (notifiedBits & NOTIFY_TIMER))
            {
                msg.id = MSG_ID_HUMIDITY;
                msg.value = (int32_t)humidity;
                xQueueSend(sensorQueueHandle, &msg, 0);
            }
        }
        else
        {
            msg.id = MSG_ID_HUMIDITY;
            msg.value = -1;
            xQueueSend(sensorQueueHandle, &msg, 0);
        }
    }
}

static void PrintTask(void *argument)
{
    SensorMsg msg;
    char buffer[48];
    int length;

    (void)argument;

    for (;;)
    {
        if (pdPASS == xQueueReceive(sensorQueueHandle, &msg, portMAX_DELAY))
        {
            if (MSG_ID_TEMPERATURE == msg.id)
            {
                length =
                    sprintf(buffer, "Temperature: %ld C\r\n", (long)msg.value);
            }
            else
            {
                length =
                    sprintf(buffer, "Humidity: %ld %%\r\n", (long)msg.value);
            }

            HAL_UART_Transmit(&huart2, (uint8_t *)buffer, (uint16_t)length,
                              HAL_MAX_DELAY);
        }
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if ((B1_BUTTON_Pin == GPIO_Pin) || (PB3_BUTTON_Pin == GPIO_Pin))
    {
        xTaskNotifyFromISR(dhtTaskHandle, NOTIFY_BUTTON, eSetBits,
                           &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (TIM6 == htim->Instance)
    {
        HAL_IncTick();
    }
    else if (TIM7 == htim->Instance)
    {
        xTaskNotifyFromISR(dhtTaskHandle, NOTIFY_TIMER, eSetBits,
                           &xHigherPriorityTaskWoken);
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}