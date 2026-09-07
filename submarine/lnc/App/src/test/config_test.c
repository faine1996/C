#include "config.h"
#include "test_bench.h"
#include <stdio.h>
#include <stdint.h>

/* -----------------------------------------------------------------------
 * Config self-test.
 * Exercises Config_ApplyParam for representative param types,
 * verifies round-trip through Flash with Config_Get,
 * then restores the original config.
 * --------------------------------------------------------------------- */

void Config_Test(void)
{
    ConfigData_t original;
    ConfigData_t result;
    uint8_t      payload[4];

    /* Save the current live config so we can restore it after the test.
     * Config_Get returns a copy by value so this is safe to call at any
     * point after Config_Init has run. */
    original = Config_Get();

    /* --- Test 1: TEMP_NORMAL — 4-byte payload, two int16 little-endian.
     * Set normal low = 15, normal high = 30. */
    payload[0] = 15U;
    payload[1] = 0U;
    payload[2] = 30U;
    payload[3] = 0U;
    Config_ApplyParam(CONFIG_PARAM_TEMP_NORMAL, payload, 4U);

    result = Config_Get();
    TEST_CHECK(result.temp_normal_low  == (int16_t)15,
               "Config TEMP_NORMAL low  == 15");
    TEST_CHECK(result.temp_normal_high == (int16_t)30,
               "Config TEMP_NORMAL high == 30");

    /* --- Test 2: HUM_NORMAL_MIN — 1-byte payload, uint8. */
    payload[0] = 40U;
    Config_ApplyParam(CONFIG_PARAM_HUM_NORMAL_MIN, payload, 1U);

    result = Config_Get();
    TEST_CHECK(result.hum_normal_min == (uint8_t)40U,
               "Config HUM_NORMAL_MIN == 40");

    /* --- Test 3: BATT_WARNING_MIN — 2-byte payload, uint16 little-endian.
     * 2000 = 0x07D0 -> payload[0]=0xD0, payload[1]=0x07 */
    payload[0] = (uint8_t)(2000U & 0xFFU);
    payload[1] = (uint8_t)((2000U >> 8U) & 0xFFU);
    Config_ApplyParam(CONFIG_PARAM_BATT_WARNING_MIN, payload, 2U);

    result = Config_Get();
    TEST_CHECK(result.batt_warning_min == (uint16_t)2000U,
               "Config BATT_WARNING_MIN == 2000");

    /* --- Test 4: magic number survives all the round-trips above. */
    TEST_CHECK(result.magic == CONFIG_MAGIC,
               "Config magic valid after writes");

    /* --- Restore: write the original config back via Config_RestoreForTest.
     * We cannot use Config_ApplyParam here because that would require one
     * call per parameter. Config_RestoreForTest writes the full snapshot
     * directly to RAM and Flash in one operation. */
    Config_RestoreForTest(&original);
    printf("[CONFIG-TEST] Original config restored\r\n");
}