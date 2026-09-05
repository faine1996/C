#ifndef BUZZER_H
#define BUZZER_H

/**
 * @brief   Initialises the buzzer PWM channel to the off state.
 *          TIM3 CH1 (PB4) is configured by CubeMX; this function
 *          ensures the channel starts stopped.
 * @param   None.
 * @retval  None.
 */
void Buzzer_Init(void);

/**
 * @brief   Starts the buzzer tone by enabling PWM output on TIM3 CH1.
 *          Returns immediately — the tone continues until Buzzer_Off()
 *          is called.
 * @param   None.
 * @retval  None.
 */
void Buzzer_On(void);

/**
 * @brief   Stops the buzzer tone by disabling PWM output on TIM3 CH1.
 * @param   None.
 * @retval  None.
 */
void Buzzer_Off(void);

/**
 * @brief   Sounds the buzzer briefly via PWM for audible confirmation,
 *          then stops it. Blocks for the duration of the beep.
 * @param   None.
 * @retval  None.
 */
void Buzzer_Test(void);

#endif /* BUZZER_H */