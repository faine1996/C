#ifndef UART_MANAGER_H
#define UART_MANAGER_H

#include "main.h" /* Pulls in UART_HandleTypeDef and other HAL types */
#include "queue.h"

/* Initializes the manager with a target UART instance and buffer size */
void UartManager_Init(UART_HandleTypeDef* huart, uint16_t queueSize);

/* Enqueues or transmits the data */
void UartManager_Send(char* data, uint16_t size);

#endif