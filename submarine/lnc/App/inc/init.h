#ifndef INIT_H
#define INIT_H

#include "Ds1307.h"
#include <stdint.h>

/**
 * @brief   Reads and clears the IWDG reset flag from RCC_CSR, stores the
 *          result for Init_Task to use when writing the startup event.
 *          Creates Init's internal time-sync queue (one slot).
 *          Call once in MX_FREERTOS_Init, after Log_Init().
 * @param   None.
 * @retval  None.
 */
void Init_Init(void);

/**
 * @brief   FreeRTOS task function for the Init module. Sends a
 *          TIME_SYNC_REQ to the Central Computer, waits up to 5 seconds
 *          for a SET_TIME response, applies the received time to the
 *          DS1307, falls back to the battery-backed RTC or the Log_Init
 *          default if no response arrives. Writes a startup event to the
 *          Log event queue. Calls osThreadExit() when done.
 * @param   argument  FreeRTOS task argument, unused.
 * @retval  None.
 */
void Init_Task(void *argument);

/**
 * @brief   Called by Comm's dispatch_command when a TAG_SET_TIME frame
 *          arrives. Posts the parsed time fields into Init's internal
 *          queue so Init_Task can apply them to the DS1307.
 * @param   t  Pointer to a Ds1307_Time_t already populated from the
 *             seven wire bytes (year, month, date, day_of_week, hours,
 *             minutes, seconds).
 * @retval  None.
 */
void Init_NotifyTimeReceived(const Ds1307_Time_t *t);

#endif /* INIT_H */