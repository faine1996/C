#include "led_control.h"
#include <string.h>

static UART_HandleTypeDef *_huart;
static uint8_t rx_byte;
static char rx_buffer[32];
static uint8_t rx_index = 0;

void LED_Control_Init(UART_HandleTypeDef *huart)
{
    _huart = huart;
    HAL_UART_Transmit(_huart, (uint8_t*)"READY\r\n", 7, 100);
    HAL_UART_Receive_IT(_huart, &rx_byte, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART2)
    {
        if (rx_byte == '\n' || rx_byte == '\r')
        {
            if (rx_index == 0)
            {
                HAL_UART_Receive_IT(_huart, &rx_byte, 1);
                return;
            }
            rx_buffer[rx_index] = '\0';

            if (strcmp(rx_buffer, "led on 1") == 0)
            {
                HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
                HAL_UART_Transmit(_huart, (uint8_t*)"OK: LED1 ON\r\n", 13, 100);
            }
            else if (strcmp(rx_buffer, "led off 1") == 0)
            {
                HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
                HAL_UART_Transmit(_huart, (uint8_t*)"OK: LED1 OFF\r\n", 14, 100);
            }
            else if (strcmp(rx_buffer, "led on 2") == 0)
            {
                HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
                HAL_UART_Transmit(_huart, (uint8_t*)"OK: LED2 ON\r\n", 13, 100);
            }
            else if (strcmp(rx_buffer, "led off 2") == 0)
            {
                HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_RESET);
                HAL_UART_Transmit(_huart, (uint8_t*)"OK: LED2 OFF\r\n", 14, 100);
            }
            else
            {
                HAL_UART_Transmit(_huart, (uint8_t*)"ERR: Unknown command\r\n", 21, 100);
            }

            rx_index = 0;
            memset(rx_buffer, 0, sizeof(rx_buffer));
        }
        else
        {
            if (rx_index < sizeof(rx_buffer) - 1)
                rx_buffer[rx_index++] = rx_byte;
        }

        HAL_UART_Receive_IT(_huart, &rx_byte, 1);
    }
}