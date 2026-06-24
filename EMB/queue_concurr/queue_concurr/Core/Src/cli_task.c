#include "cli_task.h"
#include "cmsis_os.h"
#include "main.h"
#include <string.h>

#define CLI_BUF_LEN 16

extern UART_HandleTypeDef huart2;
extern osMessageQueueId_t delayQueueHandle;

static void uart_print(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, (uint16_t)strlen(msg),
                      HAL_MAX_DELAY);
}

void Task_CLI(void *argument)
{
    char buf[CLI_BUF_LEN];
    uint8_t ch;
    int idx;
    int i;
    uint32_t val;
    uint16_t delay;

    (void)argument;

    for (;;)
    {
        uart_print("Enter delay (ms): ");

        memset(buf, 0, sizeof(buf));
        idx = 0;

        for (;;)
        {
            HAL_UART_Receive(&huart2, &ch, 1, HAL_MAX_DELAY);

            if ('\r' == ch || '\n' == ch)
            {
                uart_print("\r\n");
                break;
            }

            if (' ' <= ch && '~' >= ch && idx < (CLI_BUF_LEN - 1))
            {
                HAL_UART_Transmit(&huart2, &ch, 1, HAL_MAX_DELAY);
                buf[idx] = (char)ch;
                idx++;
            }
        }

        if (0 == idx)
        {
            continue;
        }

        val = 0U;
        for (i = 0; i < idx; i++)
        {
            if ('0' <= buf[i] && '9' >= buf[i])
            {
                val = val * 10U + (uint32_t)(buf[i] - '0');
            }
        }

        if (0U < val && 65535U >= val)
        {
            delay = (uint16_t)val;
            osMessageQueuePut(delayQueueHandle, &delay, 0, 0);
            uart_print("Delay updated\r\n");
        }
        else
        {
            uart_print("Invalid (1-65535)\r\n");
        }
    }
}