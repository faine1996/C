#ifndef MENU_H
#define MENU_H

/**
 * @brief   Prints the test menu, blocks for one keypress, and runs the
 *          matching driver self-test. Intended to be called repeatedly
 *          from a task's infinite loop.
 * @param   None.
 * @retval  None.
 */
void Menu_Run(void);

#endif /* MENU_H */