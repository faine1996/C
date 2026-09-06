#ifndef WATCHDOG_H
#define WATCHDOG_H

/**
 * @brief   Initialises and starts the hardware independent watchdog
 *          (IWDG). Call once before the FreeRTOS scheduler starts.
 * @param   None.
 * @retval  None.
 */
void Watchdog_Init(void);

/**
 * @brief   FreeRTOS task function for the Watchdog module. Refreshes
 *          the IWDG on a fixed cadence to prevent a watchdog-triggered
 *          reset. Must be the highest priority task.
 * @param   argument  FreeRTOS task argument, unused.
 * @retval  None.
 */
void Watchdog_Task(void *argument);

#endif /* WATCHDOG_H */