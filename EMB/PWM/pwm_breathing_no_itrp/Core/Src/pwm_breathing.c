#include "pwm_breathing.h"
#include "main.h"
#include <stdio.h>

extern TIM_HandleTypeDef htim2;
extern TIM_HandleTypeDef htim3;
extern UART_HandleTypeDef huart2;

void PWM_Breathing_Update(void)
{
    static int blue_duty = 1;
    static int red_duty = 99;
    static int blue_dir = 1;
    static int red_dir = -1;
    char buf[32];
    int len;

    blue_duty += blue_dir;
    red_duty += red_dir;

    if (99 <= blue_duty)
    {
        blue_duty = 99;
        blue_dir = -1;
    }
    if (1 >= blue_duty)
    {
        blue_duty = 1;
        blue_dir = 1;
    }
    if (99 <= red_duty)
    {
        red_duty = 99;
        red_dir = -1;
    }
    if (1 >= red_duty)
    {
        red_duty = 1;
        red_dir = 1;
    }

    __HAL_TIM_SET_COMPARE(&htim2, TIM_CHANNEL_1, (uint32_t)blue_duty);
    __HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, (uint32_t)red_duty);

    len = sprintf(buf, "B:%d%%  R:%d%%\r\n", blue_duty, red_duty);
    HAL_UART_Transmit(&huart2, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
}