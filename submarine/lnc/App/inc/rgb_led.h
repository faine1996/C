#ifndef RGB_LED_H
#define RGB_LED_H

typedef enum
{
    RGB_LED_MODE_NORMAL  = 0,
    RGB_LED_MODE_WARNING = 1,
    RGB_LED_MODE_ERROR   = 2,
    RGB_LED_MODE_OFF     = 3
} RgbLedMode_t;

/**
 * @brief   Initializes the RGB LED outputs to the off state.
 * @param   None.
 * @retval  None.
 */
void RgbLed_Init(void);

/**
 * @brief   Sets the RGB LED to the color matching the given mode.
 *          NORMAL = green, WARNING = yellow, ERROR = red, OFF = dark.
 * @param   mode  One of the RgbLedMode_t values.
 * @retval  None.
 */
void RgbLed_SetMode(RgbLedMode_t mode);

/**
 * @brief   Cycles the RGB LED through green, yellow, red, and off, with a
 *          delay between each, for visual confirmation on the board.
 * @param   None.
 * @retval  None.
 */
void RgbLed_Test(void);

#endif /* RGB_LED_H */