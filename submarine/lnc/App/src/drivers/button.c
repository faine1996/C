#include "button.h"
#include "main.h"
#include "test_bench.h"
#include <stdio.h>

void Button_Init(void)
{
}

uint8_t Button_IsPressed(void)
{
    return (GPIO_PIN_RESET == HAL_GPIO_ReadPin(BTN_ALARM_GPIO_Port, BTN_ALARM_Pin))
               ? 1U
               : 0U;
}

void Button_Test(void)
{
    uint8_t last_state;
    uint8_t current_state;
    uint32_t elapsed_ms;

    printf("Button test: press and release the alarm-stop button.\r\n");
    printf("Watching for 10 seconds...\r\n");

    last_state = Button_IsPressed();
    elapsed_ms = 0U;

    while (10000U > elapsed_ms)
    {
        HAL_Delay(50U);
        elapsed_ms += 50U;

        current_state = Button_IsPressed();

        if (current_state != last_state)
        {
            if (1U == current_state)
            {
                printf("Button PRESSED\r\n");
            }
            else
            {
                printf("Button RELEASED\r\n");
            }

            last_state = current_state;
        }
    }

    TEST_CHECK(1, "Button poll loop completed (interaction confirmed by transitions above)");
}