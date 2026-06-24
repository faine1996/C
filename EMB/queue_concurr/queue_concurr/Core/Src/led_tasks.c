#include "led_tasks.h"
#include "cmsis_os.h"

extern osMessageQueueId_t delayQueueHandle;

BlinkTaskParams blink_params = {LED1_PORT, LED1_PIN, 500};

void Task_Blink(void *argument)
{
    uint16_t new_delay;
    osStatus_t status;

    (void)argument;

    for (;;)
    {
        status = osMessageQueueGet(delayQueueHandle, &new_delay, NULL, 0);
        if (osOK == status)
        {
            blink_params.delay_ms = (uint32_t)new_delay;
        }

        HAL_GPIO_TogglePin(blink_params.port, blink_params.pin);
        osDelay(blink_params.delay_ms);
    }
}
