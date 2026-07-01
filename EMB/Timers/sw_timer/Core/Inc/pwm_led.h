#ifndef PWM_LED_H
#define PWM_LED_H

#include "main.h"

typedef struct
{
    TIM_HandleTypeDef* htim;
    uint32_t channel;
    uint32_t period;
    uint8_t brightness;
} PwmLed;

void PwmLed_Init(PwmLed* self, TIM_HandleTypeDef* htim, uint32_t channel);
void PwmLed_SetBrightness(PwmLed* self, uint8_t percent);
uint8_t PwmLed_GetBrightness(PwmLed* self);

#endif /* PWM_LED_H */