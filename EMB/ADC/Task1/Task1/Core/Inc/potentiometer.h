#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

#include "stm32l4xx_hal.h"

typedef struct
{
    ADC_HandleTypeDef *hadc;
    uint32_t           last_value;
} Potentiometer;

void     Potentiometer_Init    (Potentiometer *self, ADC_HandleTypeDef *hadc);
uint32_t Potentiometer_Read    (Potentiometer *self);
uint32_t Potentiometer_GetValue(const Potentiometer *self);

#endif /* POTENTIOMETER_H */