#include "light_sensor.h"

void LightSensor_Init(LightSensor *self, ADC_HandleTypeDef *hadc)
{
    if (NULL == self)
    {
        return;
    }
    self->hadc = hadc;
    self->last_value = 0U;
    HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED);
}

void LightSensor_Start(LightSensor *self)
{
    if (NULL == self)
    {
        return;
    }
    HAL_ADC_Start_IT(self->hadc);
}

void LightSensor_OnConversionComplete(LightSensor *self)
{
    if (NULL == self)
    {
        return;
    }
    self->last_value = HAL_ADC_GetValue(self->hadc);
}

uint32_t LightSensor_GetValue(const LightSensor *self)
{
    if (NULL == self)
    {
        return 0U;
    }
    return self->last_value;
}