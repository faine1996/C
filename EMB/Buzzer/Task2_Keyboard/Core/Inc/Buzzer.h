#ifndef BUZZER_H
#define BUZZER_H

#include "stm32l4xx_hal.h"
#include "FreeRTOS.h"
#include "timers.h"

void Buzzer_Init(TIM_HandleTypeDef *htim, uint32_t channel);
void Buzzer_PlayNote(uint16_t freq_hz, uint32_t duration_ms);
void Buzzer_Stop(void);

#endif /* BUZZER_H */