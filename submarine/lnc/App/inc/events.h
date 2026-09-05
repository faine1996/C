#ifndef EVENT_H
#define EVENT_H

#include "monitor.h"

/* -----------------------------------------------------------------------
 * Public interface
 * --------------------------------------------------------------------- */

/**
 * @brief   Initialises Event module internal state. Initialises the
 *          Object Detection module. Call once before Event_Run().
 * @param   None.
 * @retval  None.
 */
void Event_Init(void);

/**
 * @brief   Samples all sensors, checks for mode transitions and object
 *          detection events, drives the RGB LED and buzzer accordingly,
 *          polls the alarm-stop button if the alarm is active, and prints
 *          event and send-to-CC stubs over UART. Contains the 5-second
 *          polling cadence internally via a 50 x 100 ms inner loop.
 *          In Stage 4 this function becomes a FreeRTOS task body with
 *          osDelay replacing HAL_Delay.
 * @param   None.
 * @retval  None.
 */
void Event_Run(void);

/**
 * @brief   Returns 1 if the system is currently in the suppressed state
 *          (Error mode or active object detection alarm), 0 otherwise.
 *          Future modules check this before performing non-essential work.
 * @param   None.
 * @retval  1 if suppressed, 0 if not.
 */
uint8_t Event_IsSuppressed(void);

#endif /* EVENT_H */