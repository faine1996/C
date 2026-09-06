#ifndef KEEP_ALIVE_H
#define KEEP_ALIVE_H

#include "monitor.h"
#include "cmsis_os.h"

/**
 * @brief   Initialises the Keep-Alive module. Call once before the
 *          FreeRTOS scheduler starts.
 * @param   None.
 * @retval  None.
 */
void KeepAlive_Init(void);

/**
 * @brief   FreeRTOS task function for the Keep-Alive module. Blocks on
 *          the Keep-Alive queue, and on each received MonitorData_t
 *          prints a KEEPALIVE frame stub over UART. In a later stage
 *          this stub is replaced by a real TLV frame transmission.
 * @param   argument  FreeRTOS task argument, unused.
 * @retval  None.
 */
void KeepAlive_Task(void *argument);

/**
 * @brief   Returns the Keep-Alive queue handle for Monitor to post to.
 * @param   None.
 * @retval  osMessageQueueId_t handle.
 */
osMessageQueueId_t KeepAlive_GetQueueHandle(void);

#endif /* KEEP_ALIVE_H */