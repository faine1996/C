#ifndef LIGHT_SENSOR_H
#define LIGHT_SENSOR_H

#include "stm32l4xx_hal.h"

typedef struct
{
    ADC_HandleTypeDef *hadc;
    volatile uint32_t  last_value;
} LightSensor;

void     LightSensor_Init                (LightSensor *self, ADC_HandleTypeDef *hadc);
void     LightSensor_Start               (LightSensor *self);
void     LightSensor_OnConversionComplete(LightSensor *self);
uint32_t LightSensor_GetValue            (const LightSensor *self);

#endif /* LIGHT_SENSOR_H */