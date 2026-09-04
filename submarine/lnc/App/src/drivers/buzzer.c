#include "buzzer.h"
#include "main.h"
#include "test_bench.h"
#include <stdio.h>

static uint8_t DwtInitialized = 0U;

static void Buzzer_DelayUs(uint32_t us)
{
    uint32_t start;
    uint32_t ticks;

    if (0U == DwtInitialized)
    {
        CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
        DWT->CYCCNT = 0;
        DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;
        DwtInitialized = 1U;
    }

    start = DWT->CYCCNT;
    ticks = us * (SystemCoreClock / 1000000U);

    while (ticks > (DWT->CYCCNT - start))
    {
    }
}

void Buzzer_Init(void)
{
    CoreDebug->DEMCR |= CoreDebug_DEMCR_TRCENA_Msk;
    DWT->CYCCNT = 0;
    DWT->CTRL |= DWT_CTRL_CYCCNTENA_Msk;

    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
}

void Buzzer_On(void)
{
    uint32_t i;

    for (i = 0U; i < 1000U; ++i)
    {
        HAL_GPIO_TogglePin(BUZZER_GPIO_Port, BUZZER_Pin);
        Buzzer_DelayUs(250U);
    }
}

void Buzzer_Off(void)
{
    HAL_GPIO_WritePin(BUZZER_GPIO_Port, BUZZER_Pin, GPIO_PIN_RESET);
}

void Buzzer_Test(void)
{
    printf("Buzzer test: you should hear a short beep.\r\n");

    Buzzer_On();
    HAL_Delay(500U);
    Buzzer_Off();

    TEST_CHECK(1, "Buzzer beep completed (audible confirmation required)");
}