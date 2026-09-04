#include "ir_receiver.h"
#include "main.h"
#include "test_bench.h"
#include <stdio.h>

static volatile uint8_t ObjectDetected = 0U;
static volatile uint32_t DetectionCount = 0U;

void Ir_Init(void)
{
    ObjectDetected = 0U;
    DetectionCount = 0U;
}

uint8_t Ir_IsObjectDetected(void)
{
    return (ObjectDetected);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (IR_OBJECT_Pin == GPIO_Pin)
    {
        ObjectDetected = 1U;
        ++DetectionCount;
    }
}

void Ir_Test(void)
{
    uint32_t last_count;
    uint32_t seconds;

    printf("IR test: point a remote at the sensor and press a button.\r\n");
    printf("Watching for 10 seconds...\r\n");

    last_count = DetectionCount;

    for (seconds = 0U; seconds < 10U; ++seconds)
    {
        HAL_Delay(1000U);

        if (DetectionCount != last_count)
        {
            printf("Detected! (count = %lu)\r\n", (unsigned long)DetectionCount);
            last_count = DetectionCount;
        }
    }

    TEST_CHECK(DetectionCount > 0U, "IR receiver detected at least one signal");
}