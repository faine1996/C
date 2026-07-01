#ifndef TIMER_APP_H
#define TIMER_APP_H
#include <stdint.h>

void TimerApp_Init(void);
void TimerApp_FlashLed2(void);
uint8_t TimerApp_GetLed1Brightness(void);

#endif /* TIMER_APP_H */