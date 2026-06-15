#include "uart_manager.h"

/* Static variables keep the state private to this file */
static Queue* s_txQueue = NULL;
static UART_HandleTypeDef* s_huart = NULL;

void UartManager_Init(UART_HandleTypeDef* huart, uint16_t queueSize)
{
    if ( NULL != huart )
    {
        s_huart = huart;
        s_txQueue = Queue_create(queueSize);
    }
}

void UartManager_Send(char* data, uint16_t size)
{
    if ( NULL == s_huart || NULL == s_txQueue || NULL == data )
    {
        return;
    }

    /* Crocodile check: Hardware is free, transmit immediately */
    if ( HAL_UART_STATE_READY == s_huart->gState )
    {
        HAL_UART_Transmit_IT(s_huart, (uint8_t*)data, size);
    }
    else
    {
        /* Hardware is busy, copy to ring buffer */
        Queue_enque(s_txQueue, data, size);
    }
}

/* * The HAL framework declares this as __weak internally. 
 * By defining it here, the linker routes the hardware interrupt to this file. 
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    static char nextChar;
    
    /* Ensure the interrupt is from our managed UART instance */
    if ( huart->Instance == s_huart->Instance )
    {
        if ( 0 < Queue_size(s_txQueue) )
        {
            nextChar = Queue_getChar(s_txQueue);
            
            /* Send the next byte directly from the queue */
            HAL_UART_Transmit_IT(huart, (uint8_t*)&nextChar, 1);
        }
    }
}