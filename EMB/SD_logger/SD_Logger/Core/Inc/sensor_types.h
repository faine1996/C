#ifndef __SENSOR_TYPES_H
#define __SENSOR_TYPES_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

typedef struct
{
    uint8_t temperature;
    uint8_t humidity;
    uint32_t tick;
}
DhtReading_t;

#ifdef __cplusplus
}
#endif

#endif /* __SENSOR_TYPES_H */