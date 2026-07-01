#ifndef GPIO_LED_H
#define GPIO_LED_H

#include "main.h"

typedef struct
{
    GPIO_TypeDef* port;
    uint16_t pin;
} GpioLed;

void GpioLed_Init(GpioLed* self, GPIO_TypeDef* port, uint16_t pin);
void GpioLed_On(GpioLed* self);
void GpioLed_Off(GpioLed* self);

#endif /* GPIO_LED_H */