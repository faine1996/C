#include "timer_app.h"
#include "cmsis_os.h"
#include "gpio_led.h"
#include "main.h"
#include "pwm_led.h"

/* Expose Timer 2 handle from main.c */
extern TIM_HandleTypeDef htim2;

/* Configuration Constants */
#define DIM_STEP_PERCENT (10U)
#define DIM_PERIOD_TICKS (1500U)
#define LED2_OFF_DELAY_TICKS (500U)

/* File-scope Application State */
static PwmLed led1_pwm;
static GpioLed led2_gpio;
static osTimerId_t dimTimerHandle;
static osTimerId_t led2OffTimerHandle;

/* Callback Prototypes */
static void DimTimerCallback(void *argument);
static void Led2OffTimerCallback(void *argument);

void TimerApp_Init(void)
{
    /* Initialize Hardware Wrappers */
    PwmLed_Init(&led1_pwm, &htim2, TIM_CHANNEL_1);
    GpioLed_Init(&led2_gpio, LED2_GPIO_Port, LED2_Pin);

    /* Allocate SW Timers */
    dimTimerHandle = osTimerNew(DimTimerCallback, osTimerPeriodic, NULL, NULL);
    led2OffTimerHandle =
        osTimerNew(Led2OffTimerCallback, osTimerOnce, NULL, NULL);

    /* Start the periodic background timer */
    if (NULL != dimTimerHandle)
    {
        osTimerStart(dimTimerHandle, DIM_PERIOD_TICKS);
    }
}

void TimerApp_FlashLed2(void)
{
    GpioLed_On(&led2_gpio);

    if (NULL != led2OffTimerHandle)
    {
        /* Arm the one-shot timer; hardware turns off in background */
        osTimerStart(led2OffTimerHandle, LED2_OFF_DELAY_TICKS);
    }
}

static void DimTimerCallback(void *argument)
{
    uint8_t currentBrightness;

    /* Unused parameter explicitly cast to void */
    (void)argument;

    currentBrightness = PwmLed_GetBrightness(&led1_pwm);

    if (0U == currentBrightness)
    {
        PwmLed_SetBrightness(&led1_pwm, 100U);
    }
    else
    {
        PwmLed_SetBrightness(&led1_pwm, currentBrightness - DIM_STEP_PERCENT);
    }
}

static void Led2OffTimerCallback(void *argument)
{
    (void)argument;

    GpioLed_Off(&led2_gpio);
}

uint8_t TimerApp_GetLed1Brightness(void)
{
    return PwmLed_GetBrightness(&led1_pwm);
}