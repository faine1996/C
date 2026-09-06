#include "keep_alive.h"
#include "events.h"
#include <stdio.h>

/* Queue handle — created in KeepAlive_Init, read by Monitor to post */
static osMessageQueueId_t s_queue;

void KeepAlive_Init(void)
{
    /* Queue depth 1: Monitor posts at 5s, Keep-Alive sends at 6s.
     * A depth of 1 ensures Keep-Alive always has the latest reading
     * and never accumulates stale entries. If Monitor posts before
     * Keep-Alive has consumed the previous message, the old message
     * is overwritten — acceptable since only the latest reading matters
     * for a keep-alive heartbeat. */
    s_queue = osMessageQueueNew(1U, sizeof(MonitorData_t), NULL);
}

osMessageQueueId_t KeepAlive_GetQueueHandle(void)
{
    return s_queue;
}

void KeepAlive_Task(void *argument)
{
    MonitorData_t data;
    osStatus_t    status;

    (void)argument;

    for (;;)
    {
        /* Block until Monitor posts a reading — timeout 10s as a safety
         * net in case Monitor misses a cycle. */
        status = osMessageQueueGet(s_queue,
                                   &data,
                                   NULL,
                                   pdMS_TO_TICKS(10000U));

        if (osOK == status)
        {
            /* Stub — replace with real TLV frame transmission in
             * Stage 5 (Comm module). */
            printf("[KEEPALIVE] TAG=0x10 ts=0 temp=%d hum=%u "
                   "batt=%u light=%u mode=%d\r\n",
                   (int)data.temperature,
                   (unsigned)data.humidity,
                   (unsigned)data.battery,
                   (unsigned)data.light,
                   (int)data.system_mode);
        }
        else
        {
            printf("[KEEPALIVE] WARNING: queue timeout — Monitor may "
                   "have missed a cycle.\r\n");
        }
    }
}