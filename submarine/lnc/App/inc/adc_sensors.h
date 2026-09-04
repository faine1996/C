#ifndef ADC_SENSORS_H
#define ADC_SENSORS_H

#include <stdint.h>

/**
 * @brief   Placeholder for any one-time ADC setup beyond what CubeMX
 *          already generated. Currently a no-op, kept for symmetry with
 *          the other drivers and in case future setup is needed.
 * @param   None.
 * @retval  None.
 */
void Adc_Init(void);

/**
 * @brief   Reads the raw battery voltage (potentiometer, ADC1 IN5).
 * @param   None.
 * @retval  Raw 12-bit ADC value (0-4095), or 0xFFFF on conversion error.
 */
uint16_t Adc_ReadBattery(void);

/**
 * @brief   Reads the raw light level (LDR, ADC1 IN6).
 * @param   None.
 * @retval  Raw 12-bit ADC value (0-4095), or 0xFFFF on conversion error.
 */
uint16_t Adc_ReadLight(void);

/**
 * @brief   Reads both ADC channels once and prints the raw values.
 * @param   None.
 * @retval  None.
 */
void Adc_Test(void);

#endif /* ADC_SENSORS_H */