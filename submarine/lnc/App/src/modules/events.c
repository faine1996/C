#include "events.h"
#include "object_detection.h"
#include "rgb_led.h"
#include "buzzer.h"
#include "button.h"
#include "monitor.h"
#include "ir_receiver.h"
#include "stm32l4xx_hal.h"
#include "cmsis_os.h"
#include <stdio.h>

/* -----------------------------------------------------------------------
 * Internal state
 * --------------------------------------------------------------------- */

static MonitorZone_t s_prev_mode;
static MonitorZone_t s_pre_alarm_mode;
static uint8_t       s_alarm_active;
static uint8_t       s_suppressed;

/* -----------------------------------------------------------------------
 * Constants
 * --------------------------------------------------------------------- */

/* Number of 100 ms polls that make up one 5-second Monitor sample cycle */
#define POLL_TICKS        50U
#define POLL_INTERVAL_MS  100U

/* -----------------------------------------------------------------------
 * Static variables
 * --------------------------------------------------------------------- */

static osMessageQueueId_t s_queue;

/* -----------------------------------------------------------------------
 * Static helpers
 * --------------------------------------------------------------------- */

static void stop_alarm(void)
{
    Buzzer_Off();
    s_alarm_active = 0U;
}

static void handle_mode_transition(MonitorZone_t prev,
                                   MonitorZone_t next,
                                   MonitorData_t *data)
{
    /* Suppress unused-parameter warning for data — used by future
     * send-to-CC stub once comm module exists. */
    (void)data;

    if (next == ZONE_ERROR)
    {
        s_pre_alarm_mode = prev;
        RgbLed_SetMode(RGB_LED_MODE_ERROR);
        Buzzer_On();
        s_alarm_active = 1U;
        s_suppressed   = 1U;

        printf("[EVENT] Mode -> ERROR (prev=%d). "
               "Alarm ON. System suppressed.\r\n", (int)prev);
        printf("[TO-CC] TAG=0x11 EVENT_BLOCK: MODE_CHANGE -> ERROR\r\n");
    }
    else if (next == ZONE_WARNING)
    {
        RgbLed_SetMode(RGB_LED_MODE_WARNING);

        if (s_alarm_active)
        {
            stop_alarm();
        }

        s_suppressed = 0U;

        printf("[EVENT] Mode -> WARNING (prev=%d). "
               "Alarm off. System resumed.\r\n", (int)prev);
        printf("[TO-CC] TAG=0x11 EVENT_BLOCK: MODE_CHANGE -> WARNING\r\n");
    }
    else
    {
        /* ZONE_NORMAL */
        RgbLed_SetMode(RGB_LED_MODE_NORMAL);

        if (s_alarm_active)
        {
            stop_alarm();
        }

        s_suppressed = 0U;

        printf("[EVENT] Mode -> NORMAL (prev=%d). "
               "Alarm off. System resumed.\r\n", (int)prev);
        printf("[TO-CC] TAG=0x11 EVENT_BLOCK: MODE_CHANGE -> NORMAL\r\n");
    }
}

static void handle_object_event(ObjDetEvent_t ev)
{
    if (OBJDET_DETECTED == ev)
    {
        s_pre_alarm_mode = s_prev_mode; 
        RgbLed_SetMode(RGB_LED_MODE_ERROR);
        Buzzer_On();
        s_alarm_active = 1U;
        s_suppressed   = 1U;

        printf("[EVENT] Object DETECTED. Alarm ON. System suppressed.\r\n");
        printf("[TO-CC] TAG=0x11 EVENT_BLOCK: OBJECT_DETECTED\r\n");
    }
    else if (OBJDET_CLEARED == ev)
    {
        RgbLed_SetMode(RGB_LED_MODE_NORMAL);

        if (s_alarm_active)
        {
            stop_alarm();
        }

        s_suppressed = 0U;
        Ir_Clear();
        ObjDet_Reset();

        printf("[EVENT] Object CLEARED. Alarm off. System resumed.\r\n");
        printf("[TO-CC] TAG=0x11 EVENT_BLOCK: OBJECT_CLEARED\r\n");
    }
}

static void poll_button_if_alarm(void)
{
    static uint8_t prev_pressed = 0U;
    uint8_t        pressed;

    pressed = Button_IsPressed();

    /* Act on the rising edge — button was up, now is down. */
    if (pressed && !prev_pressed)
    {
        stop_alarm();
        RgbLed_SetMode((RgbLedMode_t)s_pre_alarm_mode);
        Ir_Clear();
        ObjDet_Reset();
        s_suppressed = 0U;

        printf("[EVENT] Alarm stopped by button press.\r\n");
    }

    prev_pressed = pressed;
}

/* -----------------------------------------------------------------------
 * Public functions
 * --------------------------------------------------------------------- */

void Event_Init(void)
{
    s_queue        = osMessageQueueNew(1U, sizeof(MonitorData_t), NULL);
    s_prev_mode    = ZONE_NORMAL;
    s_pre_alarm_mode = ZONE_NORMAL;
    s_alarm_active = 0U;
    s_suppressed   = 0U;

    ObjDet_Init();
}

void Event_Task(void *argument)
{
    MonitorData_t data;
    ObjDetEvent_t obj_ev;
    osStatus_t    status;
    uint32_t      tick;

    (void)argument;

    for (;;)
    {
        status = osMessageQueueGet(s_queue,
                                   &data,
                                   NULL,
                                   pdMS_TO_TICKS(10000U));

        if (osOK != status)
        {
            printf("[EVENT] WARNING: queue timeout.\r\n");
            continue;
        }

        if (data.system_mode != s_prev_mode)
        {
            handle_mode_transition(s_prev_mode, data.system_mode, &data);
        }

        s_prev_mode = data.system_mode;

        ObjDet_Poll(&obj_ev);

        if (OBJDET_NONE != obj_ev)
        {
            handle_object_event(obj_ev);
        }

        for (tick = 0U; tick < POLL_TICKS; ++tick)
        {
            osDelay(pdMS_TO_TICKS(POLL_INTERVAL_MS));

            if (s_alarm_active)
            {
                poll_button_if_alarm();
            }
        }
    }
}

uint8_t Event_IsSuppressed(void)
{
    return s_suppressed;
}

osMessageQueueId_t Event_GetQueueHandle(void)
{
    return s_queue;
}