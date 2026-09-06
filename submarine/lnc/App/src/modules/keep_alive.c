#include "keep_alive.h"
#include "events.h"
#include "comm.h"
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
            CommMsg_t msg;
            msg.type            = COMM_MSG_KEEPALIVE;
            msg.payload.monitor = data;
            osMessageQueuePut(Comm_GetTxQueueHandle(),
                              &msg,
                              0U,
                              0U);
        }
    }
}