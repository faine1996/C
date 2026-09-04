#ifndef DHT11_H
#define DHT11_H

#include <stdint.h>

typedef enum
{
    DHT11_OK               = 0,
    DHT11_ERROR_TIMEOUT    = 1,
    DHT11_ERROR_CHECKSUM   = 2
} Dht11Status_t;

/**
 * @brief   Initializes the DHT11 data line and the DWT delay helper.
 * @param   None.
 * @retval  None.
 */
void Dht11_Init(void);

/**
 * @brief   Performs one DHT11 read (temperature + humidity).
 * @param   temperature  Output: temperature in whole degrees C.
 * @param   humidity     Output: relative humidity in whole percent.
 * @retval  DHT11_OK on success, or an error code if the read failed.
 */
Dht11Status_t Dht11_Read(int8_t *temperature, uint8_t *humidity);

/**
 * @brief   Reads the DHT11 once and prints the result over UART.
 * @param   None.
 * @retval  None.
 */
void Dht11_Test(void);

#endif /* DHT11_H */