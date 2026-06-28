#include "dht_task.h"
#include "FreeRTOS.h"
#include "cmsis_os.h"
#include "dht11.h"
#include "main.h"

void DHTTask(void *argument)
{
    DHT11_t dht;
    uint8_t temp;
    uint8_t hum;
    (void)argument;

    DHT11_Init(&dht, DHT_PIN_GPIO_Port, DHT_PIN_Pin);

    for (;;)
    {
        osEventFlagsWait(xEventGroup, FLAG_DHT, osFlagsWaitAny, osWaitForever);
        if (DHT11_OK == DHT11_Read(&dht, &temp, &hum))
        {
            g_temperature = temp;
            g_humidity = hum;
        }
        osEventFlagsSet(xEventGroup, FLAG_PRINT);
    }
}