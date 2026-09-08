#include "init.h"
#include "comm.h"
#include "log.h"
#include "Ds1307.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"

/* -----------------------------------------------------------------------
 * Constants
 * --------------------------------------------------------------------- */

/* How long to wait for SET_TIME response before falling back */
#define INIT_TIME_SYNC_TIMEOUT_MS  5000U

/* -----------------------------------------------------------------------
 * Internal state
 * --------------------------------------------------------------------- */

/* One-slot queue: Comm posts a Ds1307_Time_t here when SET_TIME arrives */
static osMessageQueueId_t s_time_queue;

/* Set in Init_Init() from RCC reset flags, read in Init_Task() */
static uint8_t s_was_wd_reset;

/* -----------------------------------------------------------------------
 * Public functions
 * --------------------------------------------------------------------- */

void Init_Init(void)
{
    /* Read the IWDG reset flag before anything can clear it */
    if (__HAL_RCC_GET_FLAG(RCC_FLAG_IWDGRST))
    {
        s_was_wd_reset = 1U;
    }
    else
    {
        s_was_wd_reset = 0U;
    }

    /* Clear all reset flags so the next boot gets a fresh reading */
    __HAL_RCC_CLEAR_RESET_FLAGS();

    /* One slot: we only care about the first SET_TIME that arrives */
    s_time_queue = osMessageQueueNew(1U, sizeof(Ds1307_Time_t), NULL);
}

void Init_NotifyTimeReceived(const Ds1307_Time_t *t)
{
    /* Called from Comm task context — queue post is task-safe.
     * osMessageQueuePut with timeout 0: if the slot is already full
     * (duplicate SET_TIME) we simply discard the second one. */
    osMessageQueuePut(s_time_queue, t, 0U, 0U);
}

void Init_Task(void *argument)
{
    CommMsg_t           msg;
    Ds1307_Time_t       t;
    CommEventPayload_t  startup_event;
    osStatus_t          status;
    uint8_t             is_set;

    (void)argument;

    /* --- Step 1: request time sync from Central Computer --- */
    msg.type = COMM_MSG_TIME_SYNC_REQ;
    osMessageQueuePut(Comm_GetTxQueueHandle(), &msg, 0U, 0U);

    /* --- Step 2: wait for SET_TIME response --- */
    status = osMessageQueueGet(s_time_queue,
                               &t,
                               NULL,
                               pdMS_TO_TICKS(INIT_TIME_SYNC_TIMEOUT_MS));

    if (osOK == status)
    {
        /* CC responded — write the received time to DS1307 */
        (void)Ds1307_SetTime(&t);
    }
    else
    {
        /* Timeout — check if DS1307 already has battery-backed time.
         * If not, Log_Init() already wrote a default — nothing more to do. */
        is_set = 0U;
        (void)Ds1307_IsTimeSet(&is_set);
    }

    /* --- Step 3: write startup event to log and notify Central Computer --- */
    startup_event.event_type = COMM_EVENT_STARTUP;
    startup_event.detail     = s_was_wd_reset;   /* 1 = WD reset, 0 = normal */
    startup_event.timestamp  = HAL_GetTick() / 1000U;

    osMessageQueuePut(Log_GetEventQueueHandle(),
                      &startup_event,
                      0U,
                      0U);

    msg.type          = COMM_MSG_EVENT;
    msg.payload.event = startup_event;
    osMessageQueuePut(Comm_GetTxQueueHandle(), &msg, 0U, 0U);

    /* --- Done: this task's job is finished --- */
    osThreadExit();
}