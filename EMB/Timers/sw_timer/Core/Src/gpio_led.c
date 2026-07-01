#include "gpio_led.h"

void GpioLed_Init(GpioLed *self, GPIO_TypeDef *port, uint16_t pin)
{
    if (NULL == self)
    {
        return;
    }

    self->port = port;
    self->pin = pin;

    GpioLed_Off(self);
}

void GpioLed_On(GpioLed *self)
{
    if (NULL == self)
    {
        return;
    }

    HAL_GPIO_WritePin(self->port, self->pin, GPIO_PIN_SET);
}

void GpioLed_Off(GpioLed *self)
{
    if (NULL == self)
    {
        return;
    }

    HAL_GPIO_WritePin(self->port, self->pin, GPIO_PIN_RESET);
}