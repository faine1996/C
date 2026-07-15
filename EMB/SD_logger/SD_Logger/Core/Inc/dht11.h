#ifndef __DHT11_H
#define __DHT11_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sensor_types.h"

typedef enum
{
    DHT11_OK = 0,
    DHT11_ERROR_TIMEOUT,
    DHT11_ERROR_CHECKSUM
}
DHT11_Status_t;

void DHT11_Init(void);
DHT11_Status_t DHT11_Read(DhtReading_t *reading);

#ifdef __cplusplus
}
#endif

#endif /* __DHT11_H */