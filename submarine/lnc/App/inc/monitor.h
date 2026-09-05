#ifndef MONITOR_H
#define MONITOR_H

#include <stdint.h>

/* -----------------------------------------------------------------------
 * Types
 * --------------------------------------------------------------------- */

typedef enum
{
    ZONE_NORMAL  = 0,
    ZONE_WARNING = 1,
    ZONE_ERROR   = 2
} MonitorZone_t;

typedef struct
{
    int8_t        temperature;
    uint8_t       humidity;
    uint16_t      battery;
    uint16_t      light;

    MonitorZone_t temp_zone;
    MonitorZone_t hum_zone;
    MonitorZone_t batt_zone;
    MonitorZone_t light_zone;

    MonitorZone_t system_mode;
} MonitorData_t;

typedef enum
{
    MONITOR_OK            = 0,
    MONITOR_ERR_DHT11     = 1,
    MONITOR_ERR_ADC_BATT  = 2,
    MONITOR_ERR_ADC_LIGHT = 3
} MonitorStatus_t;

/* -----------------------------------------------------------------------
 * Public interface
 * --------------------------------------------------------------------- */

/**
 * @brief   Initialises Monitor module internal state. Call once before
 *          any call to Monitor_Sample().
 * @param   None.
 * @retval  None.
 */
void Monitor_Init(void);

/**
 * @brief   Reads all four sensors, classifies each reading into its zone
 *          (Normal / Warning / Error), and computes the combined system
 *          mode per spec section 2.10.
 * @param   out  Pointer to a MonitorData_t struct that receives all raw
 *               values, per-reading zones, and the combined system mode.
 *               On a driver error the affected fields are set to
 *               ZONE_ERROR; all other fields are still populated.
 * @retval  MONITOR_OK on full success, or the first error code encountered
 *          if a driver read failed.
 */
MonitorStatus_t Monitor_Sample(MonitorData_t *out);

/**
 * @brief   Takes one sample, prints all raw values and zones over UART,
 *          and runs TEST_CHECK assertions on the results.
 * @param   None.
 * @retval  None.
 */
void Monitor_Test(void);

#endif /* MONITOR_H */