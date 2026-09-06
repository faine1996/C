#ifndef COMM_H
#define COMM_H

#include "monitor.h"
#include <stdint.h>

/* -----------------------------------------------------------------------
 * Event type codes (from wire protocol, section ENUMERATIONS)
 * --------------------------------------------------------------------- */

#define COMM_EVENT_MODE_CHANGE      0x01U
#define COMM_EVENT_OBJECT_DETECTED  0x02U
#define COMM_EVENT_OBJECT_CLEARED   0x03U
#define COMM_EVENT_STARTUP          0x05U

/* -----------------------------------------------------------------------
 * Frame constants — exposed here so the self-test can use them
 * --------------------------------------------------------------------- */

#define COMM_SOF             0xAAU
#define COMM_MAX_VALUE_LEN   255U
#define COMM_MAX_FRAME_LEN   259U

/* TLV tag bytes (outgoing) */
#define TAG_KEEPALIVE        0x10U
#define TAG_EVENT            0x11U
#define TAG_TIME_SYNC_REQ    0x13U
#define TAG_TIME_REPORT      0x80U

/* TLV tag bytes (incoming) */
#define TAG_SET_CONFIG       0x20U
#define TAG_SET_TIME         0x21U
#define TAG_GET_TIME         0x22U
#define TAG_GET_DATA_RANGE   0x23U
#define TAG_GET_EVENTS_RANGE 0x24U

/* -----------------------------------------------------------------------
 * Types
 * --------------------------------------------------------------------- */

/*
 * A single event to report to the Central Computer.
 * Event_Task fills one of these and posts it to Comm.
 */
typedef struct
{
    uint8_t  event_type; /* one of the COMM_EVENT_* codes above    */
    uint8_t  detail;     /* new mode (0/1/2) for MODE_CHANGE,
                            0 for all other event types             */
    uint32_t timestamp;  /* seconds since boot (HAL_GetTick / 1000) */
} CommEventPayload_t;

/*
 * Tags that identify what kind of outgoing message is in the queue.
 */
typedef enum
{
    COMM_MSG_KEEPALIVE     = 0, /* post a KEEPALIVE frame (tag 0x10)       */
    COMM_MSG_EVENT         = 1, /* post an EVENT frame    (tag 0x11)       */
    COMM_MSG_TIME_SYNC_REQ = 2  /* post a TIME_SYNC_REQUEST frame (tag 0x13,
                                   no payload)                             */
} CommMsgType_t;

/*
 * The message that Event_Task and KeepAlive_Task drop into Comm's queue.
 * It carries either a full sensor reading (for KEEPALIVE)
 * or a small event description (for EVENT).
 */
typedef struct
{
    CommMsgType_t type;
    union
    {
        MonitorData_t      monitor; /* used when type == COMM_MSG_KEEPALIVE */
        CommEventPayload_t event;   /* used when type == COMM_MSG_EVENT     */
    } payload;
} CommMsg_t;

/* -----------------------------------------------------------------------
 * Public interface
 * --------------------------------------------------------------------- */

/**
 * @brief   Initialises the Comm module and creates the TX queue.
 *          Call once before the FreeRTOS scheduler starts.
 * @param   None.
 * @retval  None.
 */
void Comm_Init(void);

/**
 * @brief   Returns the Comm TX queue handle so other tasks can post
 *          outgoing messages to it.
 * @param   None.
 * @retval  osMessageQueueId_t handle.
 */
osMessageQueueId_t Comm_GetTxQueueHandle(void);

/**
 * @brief   FreeRTOS task function for the Comm module. Drains the TX
 *          queue and sends framed TLV messages over UART. Also polls
 *          for incoming frames from the Central Computer and dispatches
 *          any commands it receives.
 * @param   argument  FreeRTOS task argument, unused.
 * @retval  None.
 */
void Comm_Task(void *argument);

/**
 * @brief   Builds one outgoing TLV frame and sends it over UART.
 *          Exposed for use by the self-test.
 * @param   tag    The TLV tag byte.
 * @param   value  Pointer to the value bytes, or NULL if length is zero.
 * @param   len    Number of value bytes (0..255).
 * @retval  None.
 */
void Comm_SendFrame(uint8_t tag, const uint8_t *value, uint8_t len);

/**
 * @brief   Runs the Comm self-test: builds several frames, checks each
 *          byte of the output against expected values, and reports
 *          pass/fail via TEST_CHECK.
 * @param   None.
 * @retval  None.
 */
void Comm_Test(void);

#endif /* COMM_H */