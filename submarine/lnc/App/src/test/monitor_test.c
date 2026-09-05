#include "monitor.h"
#include "test_bench.h"
#include <stdio.h>

static const char *zone_name(MonitorZone_t zone)
{
    switch (zone)
    {
    case ZONE_NORMAL:  return "NORMAL";
    case ZONE_WARNING: return "WARNING";
    case ZONE_ERROR:   return "ERROR";
    default:           return "UNKNOWN";
    }
}

void Monitor_Test(void)
{
    MonitorData_t   data;
    MonitorStatus_t status;

    TestBench_Init();

    printf("\r\n--- Monitor self-test ---\r\n");

    status = Monitor_Sample(&data);

    /* --- Print raw readings --- */
    printf("Temperature : %d C  -> %s\r\n",
           (int)data.temperature, zone_name(data.temp_zone));

    printf("Humidity    : %u %%  -> %s\r\n",
           (unsigned)data.humidity, zone_name(data.hum_zone));

    printf("Battery     : %u    -> %s\r\n",
           (unsigned)data.battery, zone_name(data.batt_zone));

    printf("Light       : %u    -> %s\r\n",
           (unsigned)data.light, zone_name(data.light_zone));

    printf("System mode : %s\r\n", zone_name(data.system_mode));

    /* --- TEST_CHECK assertions --- */
    TEST_CHECK(status == MONITOR_OK, "all drivers read OK");

    TEST_CHECK(data.battery <= 4095U, "battery raw in range");

    TEST_CHECK(data.light <= 4095U, "light raw in range");

    TEST_CHECK(data.system_mode == ZONE_NORMAL  ||
               data.system_mode == ZONE_WARNING ||
               data.system_mode == ZONE_ERROR,
               "system mode is valid");

    TEST_CHECK(data.temp_zone == ZONE_NORMAL  ||
               data.temp_zone == ZONE_WARNING ||
               data.temp_zone == ZONE_ERROR,
               "temp zone is valid");

    TEST_CHECK(data.hum_zone == ZONE_NORMAL  ||
               data.hum_zone == ZONE_WARNING ||
               data.hum_zone == ZONE_ERROR,
               "humidity zone is valid");

    TestBench_Summary();
}