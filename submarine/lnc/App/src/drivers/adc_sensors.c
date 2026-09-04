#include "adc_sensors.h"
#include "main.h"
#include "adc.h"
#include "test_bench.h"
#include <stdio.h>

#define ADC_ERROR_VALUE 0xFFFFU
#define ADC_TIMEOUT_MS  10U

static uint16_t Adc_ReadChannel(uint32_t channel)
{
    ADC_ChannelConfTypeDef channel_config;
    uint16_t result;

    channel_config.Channel = channel;
    channel_config.Rank = ADC_REGULAR_RANK_1;
    channel_config.SamplingTime = ADC_SAMPLETIME_92CYCLES_5;
    channel_config.SingleDiff = ADC_SINGLE_ENDED;
    channel_config.OffsetNumber = ADC_OFFSET_NONE;
    channel_config.Offset = 0U;

    if (HAL_OK != HAL_ADC_ConfigChannel(&hadc1, &channel_config))
    {
        return (ADC_ERROR_VALUE);
    }

    if (HAL_OK != HAL_ADC_Start(&hadc1))
    {
        return (ADC_ERROR_VALUE);
    }

    if (HAL_OK != HAL_ADC_PollForConversion(&hadc1, ADC_TIMEOUT_MS))
    {
        (void)HAL_ADC_Stop(&hadc1);
        return (ADC_ERROR_VALUE);
    }

    result = (uint16_t)HAL_ADC_GetValue(&hadc1);

    (void)HAL_ADC_Stop(&hadc1);

    return (result);
}

void Adc_Init(void)
{
    /* ADC1 peripheral itself is already initialized by MX_ADC1_Init(). */
}

uint16_t Adc_ReadBattery(void)
{
    return (Adc_ReadChannel(ADC_CHANNEL_5));
}

uint16_t Adc_ReadLight(void)
{
    return (Adc_ReadChannel(ADC_CHANNEL_6));
}

void Adc_Test(void)
{
    uint16_t battery_raw;
    uint16_t light_raw;

    printf("ADC test: reading battery and light sensors...\r\n");

    battery_raw = Adc_ReadBattery();
    light_raw = Adc_ReadLight();

    printf("Battery (raw): %u\r\n", (unsigned int)battery_raw);
    printf("Light (raw):   %u\r\n", (unsigned int)light_raw);

    TEST_CHECK(ADC_ERROR_VALUE != battery_raw, "ADC battery channel read");
    TEST_CHECK(ADC_ERROR_VALUE != light_raw, "ADC light channel read");
}