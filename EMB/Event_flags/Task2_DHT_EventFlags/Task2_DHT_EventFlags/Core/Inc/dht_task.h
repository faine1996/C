#ifndef DHT_TASK_H
#define DHT_TASK_H

#include "cmsis_os.h"

#define FLAG_DHT    (1U << 0)
#define FLAG_PRINT  (1U << 1)

extern osEventFlagsId_t  xEventGroup;
extern volatile uint8_t  g_temperature;
extern volatile uint8_t  g_humidity;

void DHTTask(void *argument);

#endif /* DHT_TASK_H */