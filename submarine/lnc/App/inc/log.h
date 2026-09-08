#ifndef LOG_H
#define LOG_H

#include "monitor.h"
#include "comm.h"
#include <stdint.h>

/* -----------------------------------------------------------------------
 * Public interface
 * --------------------------------------------------------------------- */

/**
 * @brief   Initialises the Log module. Mounts the SD card and initialises
 *          the DS1307 RTC. If the DS1307 has not been set, loads a fixed
 *          default time. Enforces 7-file retention by deleting the oldest
 *          log file if more than 7 exist. Call once before the FreeRTOS
 *          scheduler starts.
 * @param   None.
 * @retval  None.
 */
void Log_Init(void);

/**
 * @brief   Builds a full filename from a prefix and a date string, e.g.
 *          prefix="D", date="260909" -> "D260909.csv" (with USERPath
 *          prepended). Exposed so the Comm module's GET_DATA_RANGE/
 *          GET_EVENTS_RANGE handlers can open the same files this module
 *          writes, without duplicating the naming convention.
 * @param   buf       Output buffer, at least 20 bytes.
 * @param   prefix    File prefix ("D" for data, "E" for events).
 * @param   date_str  Date string, "YYMMDD".
 * @retval  None.
 */
void Log_BuildFilename(char *buf, const char *prefix, const char *date_str);

/**
 * @brief   Appends one measurement record to today's data log file on the
 *          SD card. The filename is derived from the current DS1307 date
 *          in the format DATA_YYYYMMDD.csv. Each record is a single CSV
 *          line: timestamp_s, temp_c, humidity_pct, battery_raw,
 *          light_raw, mode.
 * @param   data  Pointer to the MonitorData_t struct to log.
 * @retval  None.
 */
void Log_WriteData(const MonitorData_t *data);

/**
 * @brief   Appends one event record to today's event log file on the SD
 *          card. The filename is derived from the current DS1307 date in
 *          the format EVNT_YYYYMMDD.csv. Each record is a single CSV
 *          line: timestamp_s, event_type, detail.
 * @param   event  Pointer to the CommEventPayload_t struct to log.
 * @retval  None.
 */
void Log_WriteEvent(const CommEventPayload_t *event);

/**
 * @brief   FreeRTOS task function for the Log module. Blocks on the
 *          Monitor queue for data records and on the Event queue for
 *          event records, writing each to the SD card.
 * @param   argument  FreeRTOS task argument, unused.
 * @retval  None.
 */
void Log_Task(void *argument);

/**
 * @brief   Returns the Log data queue handle for Monitor to post to.
 * @param   None.
 * @retval  osMessageQueueId_t handle.
 */
osMessageQueueId_t Log_GetDataQueueHandle(void);

/**
 * @brief   Returns the Log event queue handle for Event task to post to.
 * @param   None.
 * @retval  osMessageQueueId_t handle.
 */
osMessageQueueId_t Log_GetEventQueueHandle(void);

#endif /* LOG_H */