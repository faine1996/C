#include "potentiometer.h"

void Potentiometer_Init(Potentiometer *self, ADC_HandleTypeDef *hadc)
{
    if (NULL == self)
    {
        return;
    }
    self->hadc = hadc;
    self->last_value = 0U;
    HAL_ADCEx_Calibration_Start(hadc, ADC_SINGLE_ENDED);
}

uint32_t Potentiometer_Read(Potentiometer *self)
{
    uint32_t value;

    if (NULL == self)
    {
        return 0U;
    }
    if (HAL_OK != HAL_ADC_Start(self->hadc))
    {
        return self->last_value;
    }
    if (HAL_OK != HAL_ADC_PollForConversion(self->hadc, HAL_MAX_DELAY))
    {
        HAL_ADC_Stop(self->hadc);
        return self->last_value;
    }
    value = HAL_ADC_GetValue(self->hadc);
    HAL_ADC_Stop(self->hadc);
    self->last_value = value;
    return self->last_value;
}

uint32_t Potentiometer_GetValue(const Potentiometer *self)
{
    if (NULL == self)
    {
        return 0U;
    }
    return self->last_value;
}