#ifndef LED_TASKS_H
#define LED_TASKS_H

#include "cmsis_os.h"
#include "main.h"

#define LED1_PIN   LD2_Pin
#define LED1_PORT  LD2_GPIO_Port
#define LED2_PIN   GPIO_PIN_6
#define LED2_PORT  GPIOA

typedef struct
{
    GPIO_TypeDef          *port;
    uint16_t               pin;
    uint32_t               delay_ms;
    volatile unsigned long counter;
} LedTaskParams;

extern LedTaskParams led1_params;
extern LedTaskParams led2_params;

void Task_LED(void *argument);
void Task_Reporter(void *argument);

#endif /* LED_TASKS_H */