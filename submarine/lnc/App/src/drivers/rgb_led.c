#include "rgb_led.h"
#include "main.h"
#include "test_bench.h"
#include <stdio.h>

void RgbLed_Init(void)
{
    HAL_GPIO_WritePin(RGB_RED_GPIO_Port, RGB_RED_Pin, GPIO_PIN_RESET);
    HAL_GPIO_WritePin(RGB_GREEN_GPIO_Port, RGB_GREEN_Pin, GPIO_PIN_RESET);
}

void RgbLed_SetMode(RgbLedMode_t mode)
{
    GPIO_PinState red_state;
    GPIO_PinState green_state;

    red_state = GPIO_PIN_RESET;
    green_state = GPIO_PIN_RESET;

    switch (mode)
    {
    case RGB_LED_MODE_NORMAL:
        green_state = GPIO_PIN_SET;
        break;

    case RGB_LED_MODE_WARNING:
        red_state = GPIO_PIN_SET;
        green_state = GPIO_PIN_SET;
        break;

    case RGB_LED_MODE_ERROR:
        red_state = GPIO_PIN_SET;
        break;

    case RGB_LED_MODE_OFF:
    default:
        break;
    }

    HAL_GPIO_WritePin(RGB_RED_GPIO_Port, RGB_RED_Pin, red_state);
    HAL_GPIO_WritePin(RGB_GREEN_GPIO_Port, RGB_GREEN_Pin, green_state);
}

void RgbLed_Test(void)
{
    printf("RGB LED test: watch the board.\r\n");

    printf("Setting GREEN (Normal)...\r\n");
    RgbLed_SetMode(RGB_LED_MODE_NORMAL);
    HAL_Delay(1000U);

    printf("Setting YELLOW (Warning)...\r\n");
    RgbLed_SetMode(RGB_LED_MODE_WARNING);
    HAL_Delay(1000U);

    printf("Setting RED (Error)...\r\n");
    RgbLed_SetMode(RGB_LED_MODE_ERROR);
    HAL_Delay(1000U);

    printf("Setting OFF...\r\n");
    RgbLed_SetMode(RGB_LED_MODE_OFF);
    HAL_Delay(500U);

    TEST_CHECK(1, "RGB LED cycle completed (visual confirmation required)");
}