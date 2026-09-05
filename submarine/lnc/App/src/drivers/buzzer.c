#include "buzzer.h"
#include "tim.h"
#include "stm32l4xx_hal.h"

void Buzzer_Init(void)
{
    /* Ensure PWM output starts in the off state. TIM3 clock and pin
     * alternate function are already configured by MX_TIM3_Init(). */
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

void Buzzer_On(void)
{
    HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1);
}

void Buzzer_Off(void)
{
    HAL_TIM_PWM_Stop(&htim3, TIM_CHANNEL_1);
}

void Buzzer_Test(void)
{
    Buzzer_On();
    HAL_Delay(500);
    Buzzer_Off();
}