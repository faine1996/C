#include <stdint.h>
#include "stm32l4xx_hal.h"

void led_toggle_callback(void *param)
{
    (void)param;
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
}