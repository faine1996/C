#include "monitor.h"
#include "dht11.h"
#include "adc_sensors.h"
#include "cmsis_os.h"
#include "keep_alive.h"
#include "events.h"

/* -----------------------------------------------------------------------
 * Limit constants — all thresholds in one place.
 * Change a value or direction here; nothing else needs touching.
 * --------------------------------------------------------------------- */

/* Temperature (degrees C) */
#define TEMP_NORMAL_LOW    18
#define TEMP_NORMAL_HIGH   28
#define TEMP_WARNING_LOW   10
#define TEMP_WARNING_HIGH  35

/* Humidity (percent) */
#define HUM_NORMAL_LOW     30
#define HUM_NORMAL_HIGH    60
#define HUM_WARNING_HIGH   75

/* Battery (raw 12-bit ADC, HIGH = healthy) */
#define BATT_NORMAL_MIN    3000
#define BATT_WARNING_MIN   1500

/* Light (raw 12-bit ADC, HIGH = bright = Normal) */
#define LIGHT_NORMAL_MIN   700
#define LIGHT_WARNING_MIN  300

/* ADC sentinel value returned by the driver on conversion error */
#define ADC_ERROR_SENTINEL 0xFFFFU

/* -----------------------------------------------------------------------
 * Static variables
 * --------------------------------------------------------------------- */

static osMessageQueueId_t s_event_queue;
static osMessageQueueId_t s_keepalive_queue;

/* -----------------------------------------------------------------------
 * Static helpers — classify one reading into its zone
 * --------------------------------------------------------------------- */

static MonitorZone_t classify_temperature(int8_t t)
{
    if (t >= TEMP_NORMAL_LOW && t <= TEMP_NORMAL_HIGH)
    {
        return ZONE_NORMAL;
    }

    if (t >= TEMP_WARNING_LOW && t <= TEMP_WARNING_HIGH)
    {
        return ZONE_WARNING;
    }

    return ZONE_ERROR;
}

static MonitorZone_t classify_humidity(uint8_t h)
{
    if (h >= HUM_NORMAL_LOW && h <= HUM_NORMAL_HIGH)
    {
        return ZONE_NORMAL;
    }

    if (h <= HUM_WARNING_HIGH)
    {
        return ZONE_WARNING;
    }

    return ZONE_ERROR;
}

static MonitorZone_t classify_battery(uint16_t b)
{
    /* HIGH raw ADC = healthy battery (deliberate direction — see design log) */
    if (b >= BATT_NORMAL_MIN)
    {
        return ZONE_NORMAL;
    }

    if (b >= BATT_WARNING_MIN)
    {
        return ZONE_WARNING;
    }

    return ZONE_ERROR;
}

static MonitorZone_t classify_light(uint16_t l)
{
    /* HIGH raw ADC = bright = Normal (deliberate direction — see design log) */
    if (l >= LIGHT_NORMAL_MIN)
    {
        return ZONE_NORMAL;
    }

    if (l >= LIGHT_WARNING_MIN)
    {
        return ZONE_WARNING;
    }

    return ZONE_ERROR;
}

static MonitorZone_t combine_zones(MonitorZone_t temp_z,
                                   MonitorZone_t hum_z,
                                   MonitorZone_t batt_z,
                                   MonitorZone_t light_z)
{
    /* Spec section 2.10: any Error -> Error; any Warning (no Error) ->
     * Warning; all Normal -> Normal. */
    if (temp_z  == ZONE_ERROR ||
        hum_z   == ZONE_ERROR ||
        batt_z  == ZONE_ERROR ||
        light_z == ZONE_ERROR)
    {
        return ZONE_ERROR;
    }

    if (temp_z  == ZONE_WARNING ||
        hum_z   == ZONE_WARNING ||
        batt_z  == ZONE_WARNING ||
        light_z == ZONE_WARNING)
    {
        return ZONE_WARNING;
    }

    return ZONE_NORMAL;
}

/* -----------------------------------------------------------------------
 * Public functions
 * --------------------------------------------------------------------- */

void Monitor_Init(void)
{
    s_event_queue     = Event_GetQueueHandle();
    s_keepalive_queue = KeepAlive_GetQueueHandle();
}

MonitorStatus_t Monitor_Sample(MonitorData_t *out)
{
    Dht11Status_t  dht_status;
    MonitorStatus_t result;

    result = MONITOR_OK;

    /* --- DHT11: temperature + humidity --- */
    dht_status = Dht11_Read(&out->temperature, &out->humidity);

    if (DHT11_OK != dht_status)
    {
        out->temp_zone = ZONE_ERROR;
        out->hum_zone  = ZONE_ERROR;

        if (MONITOR_OK == result)
        {
            result = MONITOR_ERR_DHT11;
        }
    }
    else
    {
        out->temp_zone = classify_temperature(out->temperature);
        out->hum_zone  = classify_humidity(out->humidity);
    }

    /* --- ADC: battery --- */
    out->battery = Adc_ReadBattery();

    if (ADC_ERROR_SENTINEL == out->battery)
    {
        out->batt_zone = ZONE_ERROR;

        if (MONITOR_OK == result)
        {
            result = MONITOR_ERR_ADC_BATT;
        }
    }
    else
    {
        out->batt_zone = classify_battery(out->battery);
    }

    /* --- ADC: light --- */
    out->light = Adc_ReadLight();

    if (ADC_ERROR_SENTINEL == out->light)
    {
        out->light_zone = ZONE_ERROR;

        if (MONITOR_OK == result)
        {
            result = MONITOR_ERR_ADC_LIGHT;
        }
    }
    else
    {
        out->light_zone = classify_light(out->light);
    }

    /* --- Combine into system mode --- */
    out->system_mode = combine_zones(out->temp_zone,
                                     out->hum_zone,
                                     out->batt_zone,
                                     out->light_zone);

    return result;
}

void Monitor_Task(void *argument)
{
    MonitorData_t data;

    (void)argument;

    for (;;)
    {
        Monitor_Sample(&data);

        osMessageQueuePut(s_event_queue,
                          &data,
                          0U,
                          0U);

        osMessageQueuePut(s_keepalive_queue,
                          &data,
                          0U,
                          0U);

        osDelay(pdMS_TO_TICKS(5000U));
    }
}