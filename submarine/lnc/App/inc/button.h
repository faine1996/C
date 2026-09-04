#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>

/**
 * @brief   Placeholder init for symmetry with other drivers. GPIO config
 *          (PA10, polled input, pull-up) is already set by CubeMX.
 * @param   None.
 * @retval  None.
 */
void Button_Init(void);

/**
 * @brief   Polls the alarm-stop button's current state.
 * @param   None.
 * @retval  1 if the button is currently pressed, 0 if released.
 */
uint8_t Button_IsPressed(void);

/**
 * @brief   Polls the button for a short period, printing each press and
 *          release transition as it happens.
 * @param   None.
 * @retval  None.
 */
void Button_Test(void);

#endif /* BUTTON_H */