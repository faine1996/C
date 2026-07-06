#include "Keyboard.h"
#include "Buzzer.h"
#include "FreeRTOS.h"
#include "Melody.h"
#include "queue.h"
#include "task.h"

#define ARRAY_LEN(arr) (sizeof(arr) / sizeof((arr)[0]))
#define KEY_DURATION_MS 500U

static const uint16_t digit_to_freq[9] = {C1, D1, E1, F1, G1, A1, B1, C2, D2};

static QueueHandle_t xNoteQueue;
static uint8_t rx_byte;

void Keyboard_Init(UART_HandleTypeDef *huart)
{
    xNoteQueue = xQueueCreate(16, sizeof(uint16_t));
    HAL_UART_Receive_IT(huart, &rx_byte, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint8_t index;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if ('1' <= rx_byte && '9' >= rx_byte)
    {
        index = rx_byte - '1';

        if (ARRAY_LEN(digit_to_freq) > index)
        {
            xQueueSendFromISR(xNoteQueue, &digit_to_freq[index],
                              &xHigherPriorityTaskWoken);
        }
    }

    HAL_UART_Receive_IT(huart, &rx_byte, 1);

    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
}

void BuzzerTask(void *argument)
{
    uint16_t freq;

    (void)argument;

    for (;;)
    {
        xQueueReceive(xNoteQueue, &freq, portMAX_DELAY);
        Buzzer_PlayNote(freq, KEY_DURATION_MS);
        vTaskDelay(pdMS_TO_TICKS(KEY_DURATION_MS));
    }
}