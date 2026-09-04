#include "menu.h"
#include "main.h"
#include "usart.h"
#include "rgb_led.h"
#include "buzzer.h"
#include "dht11.h"
#include "adc_sensors.h"
#include "ir_receiver.h"
#include "button.h"
#include <stdio.h>

static void Menu_DrainRxBuffer(void)
{
    uint8_t drain_byte;

    while (RESET != __HAL_UART_GET_FLAG(&huart2, UART_FLAG_RXNE))
    {
        (void)HAL_UART_Receive(&huart2, &drain_byte, 1U, 1U);
    }

    __HAL_UART_CLEAR_OREFLAG(&huart2);
}

static void Menu_PrintOptions(void)
{
    printf("\r\n==== LNC Test Menu ====\r\n");
    printf("1) RGB LED test\r\n");
    printf("2) Buzzer test\r\n");
    printf("3) DHT11 test\r\n");
    printf("4) ADC test\r\n");
    printf("5) IR receiver test\r\n");
    printf("6) Alarm-stop button test\r\n");
    printf("Select: ");
}

static uint8_t Menu_ReadChoice(void)
{
    uint8_t rx_byte;
    HAL_StatusTypeDef status;

    rx_byte = 0U;
    status = HAL_UART_Receive(&huart2, &rx_byte, 1U, HAL_MAX_DELAY);

    if (HAL_OK != status)
    {
        __HAL_UART_CLEAR_OREFLAG(&huart2);
        rx_byte = 0U;
    }

    return rx_byte;
}

void Menu_Run(void)
{
    uint8_t choice;

    Menu_PrintOptions();
    choice = Menu_ReadChoice();
    printf("\r\n");

    Menu_DrainRxBuffer();

    switch (choice)
    {
    case '1':
        RgbLed_Test();
    break;

    case '2':
        Buzzer_Test();
    break;

    case '3':
        Dht11_Test();
    break;

    case '4':
        Adc_Test();
    break;

    case '5':
        Ir_Test();
    break;

    case '6':
        Button_Test();
    break;

    default:
        printf("Unknown option\r\n");
    break;

    }
}