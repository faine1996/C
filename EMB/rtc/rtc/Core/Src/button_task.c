#include "button_task.h"
#include "cmsis_os.h"
#include "datetime_poll.h"
#include "main.h"

#define BUTTON_FLAG_PRINT 0x00000001U

static osThreadId_t buttonTaskHandle;

static void Task_Button(void *argument);

void ButtonTask_Init(void)
{
    const osThreadAttr_t buttonTask_attributes = {
        .name = "buttonTask",
        .stack_size = 256 * 4,
        .priority = (osPriority_t)osPriorityNormal};

    buttonTaskHandle = osThreadNew(Task_Button, NULL, &buttonTask_attributes);
}

static void Task_Button(void *argument)
{
    (void)argument;

    for (;;)
    {
        osThreadFlagsWait(BUTTON_FLAG_PRINT, osFlagsWaitAny, osWaitForever);
        DateTime_Print(&g_dateTime);
    }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (B1_Pin == GPIO_Pin)
    {
        osThreadFlagsSet(buttonTaskHandle, BUTTON_FLAG_PRINT);
    }
}