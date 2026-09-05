#ifndef IR_RECEIVER_H
#define IR_RECEIVER_H

#include <stdint.h>

/**
 * @brief   Initializes the IR object-detection state. The EXTI interrupt
 *          itself is already configured by CubeMX (PB10, falling edge).
 * @param   None.
 * @retval  None.
 */
void Ir_Init(void);

/**
 * @brief   Returns whether an object is currently considered detected.
 * @param   None.
 * @retval  1 if an object is detected, 0 if not.
 */
uint8_t Ir_IsObjectDetected(void);

/**
 * @brief   Watches the IR receiver for a short period, printing each
 *          detected/cleared transition as it happens. Point a remote at
 *          the sensor and press a button to see it react live.
 * @param   None.
 * @retval  None.
 */
void Ir_Test(void);

/**
 * @brief   Clears the object-detected latch, marking the object as no
 *          longer detected. Call after the system has acted on a
 *          detection event and is ready to detect again.
 * @param   None.
 * @retval  None.
 */
void Ir_Clear(void);

#endif /* IR_RECEIVER_H */