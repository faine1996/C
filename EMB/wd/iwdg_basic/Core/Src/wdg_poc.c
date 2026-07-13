#include "wdg_poc.h"

extern IWDG_HandleTypeDef hiwdg;
extern UART_HandleTypeDef huart2;

static void WdgPoc_SimulateHang(void);

void WdgPoc_ReportResetCause(void)
{
    static const char msg_wdg[] = "Reset cause: IWDG watchdog\r\n";
    static const char msg_normal[] = "Reset cause: power-on / normal\r\n";

    if (0U != __HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
    {
        HAL_UART_Transmit(&huart2, (uint8_t *)msg_wdg,
                          (uint16_t)(sizeof(msg_wdg) - 1U), HAL_MAX_DELAY);
    }
    else
    {
        HAL_UART_Transmit(&huart2, (uint8_t *)msg_normal,
                          (uint16_t)(sizeof(msg_normal) - 1U), HAL_MAX_DELAY);
    }

    __HAL_RCC_CLEAR_RESET_FLAGS();
}

void WdgPoc_Run(void)
{
    for (;;)
    {
        HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);

        if (GPIO_PIN_RESET == HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin))
        {
            WdgPoc_SimulateHang();
        }

        HAL_IWDG_Refresh(&hiwdg);
        HAL_Delay(500U);
    }
}

static void WdgPoc_SimulateHang(void)
{
    for (;;)
    {
    }
}