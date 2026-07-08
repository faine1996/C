#include "cli_task.h"
#include "cmsis_os.h"
#include "datetime.h"
#include "main.h"
#include <stdio.h>
#include <string.h>

#define CLI_BUF_LEN 32
#define CLI_CMD_PREFIX_LEN 8
#define RX_RING_SIZE 32U

extern UART_HandleTypeDef huart2;

static uint8_t rxRingBuf[RX_RING_SIZE];
static volatile uint16_t rxHead;
static volatile uint16_t rxTail;
static uint8_t rxByte;
static osSemaphoreId_t rxByteSemHandle;

void CliRx_Init(void)
{
    rxHead = 0U;
    rxTail = 0U;

    rxByteSemHandle = osSemaphoreNew(RX_RING_SIZE, 0U, NULL);

    HAL_UART_Receive_IT(&huart2, &rxByte, 1);
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    uint16_t nextHead;

    if (&huart2 == huart)
    {
        nextHead = (uint16_t)((rxHead + 1U) % RX_RING_SIZE);

        if (nextHead != rxTail)
        {
            rxRingBuf[rxHead] = rxByte;
            rxHead = nextHead;
            osSemaphoreRelease(rxByteSemHandle);
        }

        HAL_UART_Receive_IT(&huart2, &rxByte, 1);
    }
}

static uint8_t cli_get_char(void)
{
    uint8_t ch;

    osSemaphoreAcquire(rxByteSemHandle, osWaitForever);

    ch = rxRingBuf[rxTail];
    rxTail = (uint16_t)((rxTail + 1U) % RX_RING_SIZE);

    return ch;
}

static void uart_print(const char *msg)
{
    HAL_UART_Transmit(&huart2, (uint8_t *)msg, (uint16_t)strlen(msg),
                      HAL_MAX_DELAY);
}

static void handle_setdate(const char *args)
{
    unsigned int year;
    unsigned int month;
    unsigned int day;
    unsigned int weekday;
    int fields;

    fields = sscanf(args, "%u/%u/%u-%u", &year, &month, &day, &weekday);

    if (4 != fields)
    {
        uart_print("ERR\r\n");
        return;
    }

    if (0U == DateTime_SetDate((uint8_t)year, (uint8_t)month, (uint8_t)day,
                               (uint8_t)weekday))
    {
        uart_print("OK\r\n");
    }
    else
    {
        uart_print("ERR\r\n");
    }
}

static void handle_settime(const char *args)
{
    unsigned int hours;
    unsigned int minutes;
    unsigned int seconds;
    int fields;

    fields = sscanf(args, "%u:%u:%u", &hours, &minutes, &seconds);

    if (3 != fields)
    {
        uart_print("ERR\r\n");
        return;
    }

    if (0U ==
        DateTime_SetTime((uint8_t)hours, (uint8_t)minutes, (uint8_t)seconds))
    {
        uart_print("OK\r\n");
    }
    else
    {
        uart_print("ERR\r\n");
    }
}

void Task_CLI(void *argument)
{
    char buf[CLI_BUF_LEN];
    uint8_t ch;
    int idx;

    (void)argument;

    for (;;)
    {
        memset(buf, 0, sizeof(buf));
        idx = 0;

        for (;;)
        {
            ch = cli_get_char();

            if (('\r' == ch) || ('\n' == ch))
            {
                uart_print("\r\n");
                break;
            }

            if ((' ' <= ch) && ('~' >= ch) && (idx < (CLI_BUF_LEN - 1)))
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

        if (0 == strncmp(buf, "setdate ", CLI_CMD_PREFIX_LEN))
        {
            handle_setdate(&buf[CLI_CMD_PREFIX_LEN]);
        }
        else if (0 == strncmp(buf, "settime ", CLI_CMD_PREFIX_LEN))
        {
            handle_settime(&buf[CLI_CMD_PREFIX_LEN]);
        }
        else
        {
            uart_print("ERR\r\n");
        }
    }
}