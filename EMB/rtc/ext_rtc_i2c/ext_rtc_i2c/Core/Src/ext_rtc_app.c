#include "ext_rtc_app.h"
#include "Ds1307.h"
#include "main.h"
#include <stdio.h>

extern RTC_HandleTypeDef hrtc;

void Ext_Rtc_App_Init(void)
{
    Ds1307_Time_t start_time;
    Ds1307_Time_t current_time;
    Ds1307_Status_t ds1307_status;
    HAL_StatusTypeDef sync_status;
    uint8_t is_time_set;
    RTC_TimeTypeDef sync_time = {0};
    RTC_DateTypeDef sync_date = {0};
    const char *ext_rtc_state_msg;

    ds1307_status = Ds1307_IsTimeSet(&is_time_set);

    if ((DS1307_OK == ds1307_status) && (0U == is_time_set))
    {
        start_time.seconds = 0U;
        start_time.minutes = 30U;
        start_time.hours = 14U;
        start_time.day_of_week = 1U;
        start_time.date = 13U;
        start_time.month = 7U;
        start_time.year = 26U;

        (void)Ds1307_SetTime(&start_time);

        ext_rtc_state_msg =
            "external RTC was not set, initialized to fixed start time";
    }
    else
    {
        ext_rtc_state_msg =
            "external RTC was already running, keeping existing time";
    }

    ds1307_status = Ds1307_GetTime(&current_time);

    if (DS1307_OK == ds1307_status)
    {
        sync_time.Hours = current_time.hours;
        sync_time.Minutes = current_time.minutes;
        sync_time.Seconds = current_time.seconds;
        sync_time.DayLightSaving = RTC_DAYLIGHTSAVING_NONE;
        sync_time.StoreOperation = RTC_STOREOPERATION_RESET;

        sync_date.WeekDay = current_time.day_of_week;
        sync_date.Month = current_time.month;
        sync_date.Date = current_time.date;
        sync_date.Year = current_time.year;

        sync_status = HAL_RTC_SetTime(&hrtc, &sync_time, RTC_FORMAT_BIN);

        if (HAL_OK == sync_status)
        {
            sync_status = HAL_RTC_SetDate(&hrtc, &sync_date, RTC_FORMAT_BIN);
        }

        if (HAL_OK == sync_status)
        {
            printf("%s; internal RTC synced\r\n", ext_rtc_state_msg);
        }
        else
        {
            printf("%s; internal RTC sync FAILED\r\n", ext_rtc_state_msg);
        }
    }
    else
    {
        printf("%s; external RTC read failed, internal RTC not synced\r\n",
               ext_rtc_state_msg);
    }
}

void Ext_Rtc_App_Loop(void)
{
    RTC_TimeTypeDef current_time;
    RTC_DateTypeDef current_date;
    HAL_StatusTypeDef time_status;
    HAL_StatusTypeDef date_status;

    time_status = HAL_RTC_GetTime(&hrtc, &current_time, RTC_FORMAT_BIN);
    date_status = HAL_RTC_GetDate(&hrtc, &current_date, RTC_FORMAT_BIN);

    if ((HAL_OK == time_status) && (HAL_OK == date_status))
    {
        printf("%02u:%02u:%02u  %02u/%02u/%02u\r\n", current_time.Hours,
               current_time.Minutes, current_time.Seconds, current_date.Date,
               current_date.Month, current_date.Year);
    }
    else
    {
        printf("Internal RTC read error\r\n");
    }
}