#include "datetime.h"
#include "cmsis_os.h"
#include "main.h"
#include <stdio.h>

extern RTC_HandleTypeDef hrtc;
extern UART_HandleTypeDef huart2;

static osMutexId_t dateTimeMutexHandle;

static const osMutexAttr_t dateTimeMutex_attributes = {.name = "dateTimeMutex"};

void DateTime_Init(void)
{
    dateTimeMutexHandle = osMutexNew(&dateTimeMutex_attributes);
}

void DateTime_Poll(DateTime *dt)
{
    RTC_TimeTypeDef sTime;
    RTC_DateTypeDef sDate;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t weekday;

    HAL_RTC_GetTime(&hrtc, &sTime, RTC_FORMAT_BCD);
    HAL_RTC_GetDate(&hrtc, &sDate, RTC_FORMAT_BCD);

    hours = (uint8_t)(((sTime.Hours >> 4) * 10U) + (sTime.Hours & 0x0FU));
    minutes = (uint8_t)(((sTime.Minutes >> 4) * 10U) + (sTime.Minutes & 0x0FU));
    seconds = (uint8_t)(((sTime.Seconds >> 4) * 10U) + (sTime.Seconds & 0x0FU));

    year = (uint8_t)(((sDate.Year >> 4) * 10U) + (sDate.Year & 0x0FU));
    month = (uint8_t)(((sDate.Month >> 4) * 10U) + (sDate.Month & 0x0FU));
    day = (uint8_t)(((sDate.Date >> 4) * 10U) + (sDate.Date & 0x0FU));
    weekday = sDate.WeekDay;

    osMutexAcquire(dateTimeMutexHandle, osWaitForever);
    dt->hours = hours;
    dt->minutes = minutes;
    dt->seconds = seconds;
    dt->year = year;
    dt->month = month;
    dt->day = day;
    dt->weekday = weekday;
    osMutexRelease(dateTimeMutexHandle);
}

void DateTime_Print(const DateTime *dt)
{
    DateTime local;
    char buf[32];
    int len;

    osMutexAcquire(dateTimeMutexHandle, osWaitForever);
    local = *dt;
    osMutexRelease(dateTimeMutexHandle);

    len = sprintf(buf, "%02u/%02u/%02u-%u-%02u:%02u:%02u\r\n",
                  (unsigned int)local.year, (unsigned int)local.month,
                  (unsigned int)local.day, (unsigned int)local.weekday,
                  (unsigned int)local.hours, (unsigned int)local.minutes,
                  (unsigned int)local.seconds);

    HAL_UART_Transmit(&huart2, (uint8_t *)buf, (uint16_t)len, HAL_MAX_DELAY);
}

uint8_t DateTime_SetDate(uint8_t year, uint8_t month, uint8_t day,
                         uint8_t weekday)
{
    RTC_DateTypeDef sDate;

    if ((1U > month) || (12U < month))
    {
        return 1U;
    }

    if ((1U > day) || (31U < day))
    {
        return 1U;
    }

    if ((1U > weekday) || (7U < weekday))
    {
        return 1U;
    }

    sDate.Year = year;
    sDate.Month = month;
    sDate.Date = day;
    sDate.WeekDay = weekday;

    if (HAL_OK != HAL_RTC_SetDate(&hrtc, &sDate, RTC_FORMAT_BIN))
    {
        return 1U;
    }

    return 0U;
}

uint8_t DateTime_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds)
{
    RTC_TimeTypeDef sTime;

    if (23U < hours)
    {
        return 1U;
    }

    if (59U < minutes)
    {
        return 1U;
    }

    if (59U < seconds)
    {
        return 1U;
    }

    sTime.Hours = hours;
    sTime.Minutes = minutes;
    sTime.Seconds = seconds;
    sTime.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
    sTime.StoreOperation = RTC_STOREOPERATION_RESET;

    if (HAL_OK != HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BIN))
    {
        return 1U;
    }

    return 0U;
}