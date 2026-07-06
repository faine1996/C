#ifndef SENSOR_APP_H
#define SENSOR_APP_H

#include <stdint.h>

typedef enum
{
    MSG_ID_TEMPERATURE = 0,
    MSG_ID_HUMIDITY = 1
} MsgId;

typedef struct
{
    MsgId   id;
    int32_t value;
} SensorMsg;

#define NOTIFY_BUTTON (1UL << 0)
#define NOTIFY_TIMER  (1UL << 1)

void SensorApp_Init(void);

#endif /* SENSOR_APP_H */