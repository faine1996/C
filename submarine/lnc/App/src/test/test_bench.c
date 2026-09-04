#include "test_bench.h"
#include "main.h"
#include "usart.h"
#include <stdio.h>

static uint32_t PassCount;
static uint32_t FailCount;

int __io_putchar(int ch)
{
    uint8_t c;

    c = (uint8_t)ch;
    HAL_UART_Transmit(&huart2, &c, 1U, HAL_MAX_DELAY);

    return ch;
}

void TestBench_Init(void)
{
    PassCount = 0U;
    FailCount = 0U;
}

void TestBench_RecordResult(uint8_t passed, const char *name)
{
    if (0U != passed)
    {
        printf("PASS: %s\r\n", name);
        ++PassCount;
    }
    else
    {
        printf("FAIL: %s\r\n", name);
        ++FailCount;
    }
}

void TestBench_Summary(void)
{
    printf("---- %lu passed, %lu failed ----\r\n",
           (unsigned long)PassCount, (unsigned long)FailCount);
}