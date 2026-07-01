#include "pwm_led.h"

void PwmLed_Init(PwmLed *self, TIM_HandleTypeDef *htim, uint32_t channel)
{
    if (NULL == self)
    {
        return;
    }

    self->htim = htim;
    self->channel = channel;

    /* Hardware counters begin at 0. Period is Auto-Reload Register + 1 */
    self->period = __HAL_TIM_GET_AUTORELOAD(htim) + 1U;
    self->brightness = 100U;

    HAL_TIM_PWM_Start(self->htim, self->channel);
    PwmLed_SetBrightness(self, 100U);
}

void PwmLed_SetBrightness(PwmLed *self, uint8_t percent)
{
    uint32_t compare;

    if (NULL == self)
    {
        return;
    }

    if (100U < percent)
    {
        percent = 100U;
    }

    self->brightness = percent;
    compare = ((uint32_t)percent * self->period) / 100U;

    __HAL_TIM_SET_COMPARE(self->htim, self->channel, compare);
}

uint8_t PwmLed_GetBrightness(PwmLed *self)
{
    if (NULL == self)
    {
        return 0U;
    }

    return self->brightness;
}