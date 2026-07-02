#include "Buzzer.h"

static TIM_HandleTypeDef *s_htim;
static uint32_t s_channel;
static TimerHandle_t s_timer;

static void Buzzer_TimerCB(TimerHandle_t xTimer)
{
    /* daemon-task context — HAL register write only, no blocking */
    (void)xTimer;
    Buzzer_Stop();
}

void Buzzer_Init(TIM_HandleTypeDef *htim, uint32_t channel)
{
    s_htim = htim;
    s_channel = channel;

    /* period=1 is a placeholder; xTimerChangePeriod sets real value each note
     */
    s_timer = xTimerCreate("BuzTimer", pdMS_TO_TICKS(1), pdFALSE, NULL,
                           Buzzer_TimerCB);
}

void Buzzer_PlayNote(uint16_t freq_hz, uint32_t duration_ms)
{
    uint32_t arr;
    uint32_t ccr;

    if (0 == freq_hz)
    {
        Buzzer_Stop();
        return;
    }

    /* counter clock is 1 MHz (PSC=79 on 80 MHz APB1), so period in ticks =
     * 1e6/freq */
    arr = (1000000UL / (uint32_t)freq_hz) - 1UL;
    ccr = arr / 2UL; /* 50 % duty cycle */

    __HAL_TIM_SET_AUTORELOAD(s_htim, arr);
    __HAL_TIM_SET_COMPARE(s_htim, s_channel, ccr);
    __HAL_TIM_SET_COUNTER(s_htim, 0); /* avoid glitch from stale counter */

    HAL_TIM_PWM_Start(s_htim, s_channel);

    /* (re)arm timer so a new note always resets the deadline */
    xTimerChangePeriod(s_timer, pdMS_TO_TICKS(duration_ms), 0);
    xTimerStart(s_timer, 0);
}

void Buzzer_Stop(void)
{
    HAL_TIM_PWM_Stop(s_htim, s_channel);
}