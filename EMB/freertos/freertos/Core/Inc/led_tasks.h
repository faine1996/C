#ifndef LED_TASKS_H
#define LED_TASKS_H

#include "cmsis_os.h"
#include "main.h"

#define LED2_PIN   GPIO_PIN_6
#define LED2_PORT  GPIOA

extern volatile unsigned long led1_counter;
extern volatile unsigned long led2_counter;

void Task_LED1(void *argument);
void Task_LED2(void *argument);
void Task_Reporter(void *argument);

#endif /* LED_TASKS_H */