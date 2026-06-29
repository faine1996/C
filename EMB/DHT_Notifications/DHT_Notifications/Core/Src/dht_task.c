#include "dht_task.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "dht11.h"
#include "main.h"
#include "task.h"

void DHTTask(void *argument)
{
    DHT11_t dht;
    uint8_t temp;
    uint8_t hum;
    uint32_t packed;
    (void)argument;

    DHT11_Init(&dht, DHT11_DATA_GPIO_Port, DHT11_DATA_Pin);

    for (;;)
    {
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        if (DHT11_OK == DHT11_Read(&dht, &temp, &hum))
        {
            packed = ((uint32_t)temp << 16) | (uint32_t)hum;
            xTaskNotify(printTaskHandle, packed, eSetValueWithOverwrite);
        }
    }
}