#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "stm32l4xx_hal.h"

void Keyboard_Init(UART_HandleTypeDef *huart);
void BuzzerTask(void *argument);

#endif /* KEYBOARD_H */