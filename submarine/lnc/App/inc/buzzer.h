#ifndef BUZZER_H
#define BUZZER_H

/**
 * @brief   Initializes the buzzer output to the off state.
 * @param   None.
 * @retval  None.
 */
void Buzzer_Init(void);

/**
 * @brief   Turns the buzzer on.
 * @param   None.
 * @retval  None.
 */
void Buzzer_On(void);

/**
 * @brief   Turns the buzzer off.
 * @param   None.
 * @retval  None.
 */
void Buzzer_Off(void);

/**
 * @brief   Sounds the buzzer briefly for audible confirmation on the board.
 * @param   None.
 * @retval  None.
 */
void Buzzer_Test(void);

#endif /* BUZZER_H */