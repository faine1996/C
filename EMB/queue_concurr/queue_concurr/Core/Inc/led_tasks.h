#ifndef LED_TASKS_H
#define LED_TASKS_H

#include "cmsis_os.h"
#include "main.h"

#define LED1_PIN    LD2_Pin
#define LED1_PORT   LD2_GPIO_Port

typedef struct
{
    GPIO_TypeDef *port;
    uint16_t      pin;
    uint32_t      delay_ms;
} BlinkTaskParams;

extern BlinkTaskParams blink_params;

void Task_Blink(void *argument);

#endif /* LED_TASKS_H */