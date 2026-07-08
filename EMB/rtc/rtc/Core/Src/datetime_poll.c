#include "datetime_poll.h"
#include "cmsis_os.h"

#define DATETIME_POLL_PERIOD_MS 1000U

DateTime g_dateTime;

void Task_DateTimePoll(void *argument)
{
    (void)argument;

    for (;;)
    {
        DateTime_Poll(&g_dateTime);
        osDelay(DATETIME_POLL_PERIOD_MS);
    }
}