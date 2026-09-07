#include "monitor.h"
#include "dht11.h"
#include "adc_sensors.h"
#include "cmsis_os.h"
#include "keep_alive.h"
#include "events.h"
#include "task.h"
#include "config.h"
#include "log.h"


/* ADC sentinel value returned by the driver on conversion error */
#define ADC_ERROR_SENTINEL 0xFFFFU
/* -----------------------------------------------------------------------
 * Static variables
 * --------------------------------------------------------------------- */

static osMessageQueueId_t s_event_queue;
static osMessageQueueId_t s_keepalive_queue;
static osMessageQueueId_t s_log_queue;

/* -----------------------------------------------------------------------
 * Static helpers — classify one reading into its zone
 * --------------------------------------------------------------------- */

static MonitorZone_t classify_temperature(int16_t t)
{
    ConfigData_t cfg;

    cfg = Config_Get();

    if (t >= cfg.temp_normal_low && t <= cfg.temp_normal_high)
    {
        return ZONE_NORMAL;
    }

    if (t >= cfg.temp_warning_low && t <= cfg.temp_warning_high)
    {
        return ZONE_WARNING;
    }

    return ZONE_ERROR;
}

static MonitorZone_t classify_humidity(uint8_t h)
{
    ConfigData_t cfg;

    cfg = Config_Get();

    if (h >= cfg.hum_normal_min)
    {
        return ZONE_NORMAL;
    }

    if (h >= cfg.hum_warning_min)
    {
        return ZONE_WARNING;
    }

    return ZONE_ERROR;
}

static MonitorZone_t classify_battery(uint16_t b)
{
    ConfigData_t cfg;

    cfg = Config_Get();

    if (b >= cfg.batt_normal_min)
    {
        return ZONE_NORMAL;
    }

    if (b >= cfg.batt_warning_min)
    {
        return ZONE_WARNING;
    }

    return ZONE_ERROR;
}

static MonitorZone_t classify_light(uint16_t l)
{
    ConfigData_t cfg;

    cfg = Config_Get();

    if (l >= cfg.light_normal_min)
    {
        return ZONE_NORMAL;
    }

    if (l >= cfg.light_warning_min)
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
    s_log_queue = Log_GetDataQueueHandle();
}

MonitorStatus_t Monitor_Sample(MonitorData_t *out)
{
    Dht11Status_t   dht_status;
    MonitorStatus_t result;
    int8_t          dht_temp;

    result = MONITOR_OK;

    /* --- DHT11: temperature + humidity ---
     * Suspend scheduler during the read to prevent task preemption
     * from corrupting the bit-banged timing. The read takes ~20 ms;
     * all other tasks are briefly paused for that window only. */
    vTaskSuspendAll();
    dht_status = Dht11_Read(&dht_temp, &out->humidity);
    xTaskResumeAll();

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
        out->temperature = (int16_t)dht_temp;
        out->temp_zone   = classify_temperature(out->temperature);
        out->hum_zone    = classify_humidity(out->humidity);
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
    osDelay(pdMS_TO_TICKS(3000U));
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
        osMessageQueuePut(s_log_queue,
                          &data,
                          0U,
                          0U);

        osDelay(pdMS_TO_TICKS(5000U));
    }
}