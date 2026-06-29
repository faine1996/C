#ifndef DHT_TASK_H
#define DHT_TASK_H

#include "FreeRTOS.h"
#include "task.h"

extern TaskHandle_t dhtTaskHandle;
extern TaskHandle_t printTaskHandle;

void DHTTask(void *argument);

#endif /* DHT_TASK_H */