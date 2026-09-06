#include "watchdog.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"

/* IWDG refresh interval in milliseconds. Must be less than the IWDG
 * timeout period configured below. Refresh at half the timeout period
 * as a safe margin. */
#define WATCHDOG_REFRESH_MS  500U

/* IWDG handle */
static IWDG_HandleTypeDef s_hiwdg;

void Watchdog_Init(void)
{
    /* IWDG clock source: LSI oscillator (~32 kHz on L476).
     * Prescaler 32 → IWDG clock = 32000 / 32 = 1000 Hz (1 ms per tick).
     * Reload value 1999 → timeout = 1999 + 1 = 2000 ms = 2 seconds.
     * Any task stall longer than 2 seconds triggers a reset. */
    s_hiwdg.Instance       = IWDG;
    s_hiwdg.Init.Prescaler = IWDG_PRESCALER_32;
    s_hiwdg.Init.Reload    = 1999U;
    s_hiwdg.Init.Window    = IWDG_WINDOW_DISABLE;

    if (HAL_IWDG_Init(&s_hiwdg) != HAL_OK)
    {
        /* If IWDG fails to init, hang here — safer than running without
         * watchdog protection. */
        while (1)
        {
        }
    }
}

void Watchdog_Task(void *argument)
{
    (void)argument;

    /* Init here, not in MX_FREERTOS_Init, so the watchdog only starts
     * once the scheduler is running and this task can immediately
     * refresh it on schedule. */
    Watchdog_Init();

    for (;;)
    {
        osDelay(WATCHDOG_REFRESH_MS);
        HAL_IWDG_Refresh(&s_hiwdg);
    }
}