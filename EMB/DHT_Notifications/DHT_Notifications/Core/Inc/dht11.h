#ifndef DHT11_H
#define DHT11_H

#include "stm32l4xx_hal.h"

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t      pin;
} DHT11_t;

typedef enum
{
    DHT11_OK    = 0,
    DHT11_ERROR = 1
} DHT11_Status_t;

void           DHT11_Init(DHT11_t *dht, GPIO_TypeDef *port, uint16_t pin);
DHT11_Status_t DHT11_Read(DHT11_t *dht, uint8_t *temperature, uint8_t *humidity);

#endif /* DHT11_H */