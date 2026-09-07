#include "log.h"
#include "Ds1307.h"
#include "ff.h"
#include "cmsis_os.h"
#include "stm32l4xx_hal.h"
#include "fatfs.h"
#include <stdio.h>
#include <string.h>

/* -----------------------------------------------------------------------
 * Constants
 * --------------------------------------------------------------------- */

/* Maximum number of log files kept per prefix before oldest is deleted */
#define LOG_MAX_FILES        7U

/* Queue depths */
#define LOG_DATA_QUEUE_DEPTH  4U
#define LOG_EVENT_QUEUE_DEPTH 4U

/* File name prefixes — kept to 5 chars so full name fits 8.3 format */
#define LOG_DATA_PREFIX      "D"
#define LOG_EVNT_PREFIX      "E"

/* -----------------------------------------------------------------------
 * Internal state
 * --------------------------------------------------------------------- */

static osMessageQueueId_t s_data_queue;
static osMessageQueueId_t s_event_queue;

/* -----------------------------------------------------------------------
 * Internal helpers
 * --------------------------------------------------------------------- */

/*
 * Reads the current date from DS1307 and formats it as YYYYMMDD
 * into buf (must be at least 9 bytes including null terminator).
 * Returns 1 on success, 0 on DS1307 read failure.
 */
static uint8_t log_get_date_str(char *buf)
{
    Ds1307_Time_t t;

    if (DS1307_OK != Ds1307_GetTime(&t))
    {
        (void)sprintf(buf, "700101");
        return 0U;
    }

    (void)sprintf(buf, "%02u%02u%02u",
                  (unsigned)t.year,
                  (unsigned)t.month,
                  (unsigned)t.date);
    return 1U;
}

/*
 * Builds a full filename from prefix and date string.
 * e.g. prefix="DATA_", date="20260907" -> "DATA_20260907.csv"
 * buf must be at least 20 bytes.
 */
static void log_build_filename(char *buf,
                               const char *prefix,
                               const char *date_str)
{
    (void)sprintf(buf, "%s%s%s.csv", USERPath, prefix, date_str);
}

/*
 * Enforces the 7-file retention limit for files matching the given
 * prefix. Lists all matching files, and if count exceeds LOG_MAX_FILES,
 * deletes the lexicographically smallest filename (oldest date).
 * FatFS f_findfirst/f_findnext are used to scan the root directory.
 */
static void log_enforce_retention(const char *prefix)
{
    DIR      dir;
    FILINFO  fno;
    char     oldest[20];
    uint8_t  count;
    uint8_t  prefix_len;
    FRESULT  res;

    count      = 0U;
    oldest[0]  = '\0';
    prefix_len = (uint8_t)strlen(prefix);

    res = f_opendir(&dir, USERPath);    
    if (FR_OK != res)
    {
        return;
    }

    for (;;)
    {
        res = f_readdir(&dir, &fno);

        /* Empty fname signals end of directory */
        if ((FR_OK != res) || (fno.fname[0] == '\0'))
        {
            break;
        }

        /* Skip entries that don't start with our prefix */
        if (strncmp(fno.fname, prefix, prefix_len) != 0)
        {
            continue;
        }

        ++count;

        /* Track lexicographically smallest matching filename */
        if ((oldest[0] == '\0') ||
            (strcmp(fno.fname, oldest) < 0))
        {
            (void)strncpy(oldest, fno.fname, sizeof(oldest) - 1U);
            oldest[sizeof(oldest) - 1U] = '\0';
        }
    }

    f_closedir(&dir);

    /* If over the limit, delete the oldest file */
    if ((count > LOG_MAX_FILES) && (oldest[0] != '\0'))
    {
        (void)f_unlink(oldest);
        printf("[LOG] Deleted oldest file: %s\r\n", oldest);
    }
}

/*
 * Opens the log file for the given filename in append mode,
 * writes one formatted line, then closes the file.
 * Returns 1 on success, 0 on any FatFS error.
 */
static uint8_t log_append_line(const char *filename, const char *line, const char *header)
{
    FIL      fil;
    FRESULT  res;
    UINT     bw;
    uint8_t  ok;
    uint16_t len;
    uint8_t  retry;

    for (retry = 0U; retry < 3U; ++retry)
    {
        res = f_open(&fil, filename, FA_OPEN_APPEND | FA_WRITE | FA_OPEN_ALWAYS);
        if (FR_OK == res)
        {
            break;
        }
        osDelay(10U);
    }

    if (FR_OK != res)
    {
        printf("[LOG] Failed to open %s (err %d)\r\n",
               filename, (int)res);
        return 0U;
    }

    /* Write header if file is empty (new file) */
    if (0U == f_size(&fil))
    {
        len = (uint16_t)strlen(header);
        f_write(&fil, header, len, &bw);
    }


    len = (uint16_t)strlen(line);
    res = f_write(&fil, line, len, &bw);
    ok  = (FR_OK == res && bw == len) ? 1U : 0U;

    if (0U == ok)
    {
        printf("[LOG] Write failed on %s\r\n", filename);
    }

    (void)f_close(&fil);
    return ok;
}

/* -----------------------------------------------------------------------
 * Public functions
 * --------------------------------------------------------------------- */

void Log_Init(void)
{
    Ds1307_Time_t default_time;
    uint8_t       is_set;

    /* Check DS1307 — if not set, write a default time */
    if (DS1307_OK == Ds1307_IsTimeSet(&is_set))
    {
        if (0U == is_set)
        {
            /* Clock was never set — load a fixed default */
            default_time.seconds    = 0U;
            default_time.minutes    = 0U;
            default_time.hours      = 0U;
            default_time.day_of_week = 1U;
            default_time.date       = 1U;
            default_time.month      = 1U;
            default_time.year       = 26U; /* 2026 */

            if (DS1307_OK == Ds1307_SetTime(&default_time))
            {
                printf("[LOG] DS1307 not set — loaded default time\r\n");
            }
            else
            {
                printf("[LOG] DS1307 set failed\r\n");
            }
        }
        else
        {
            printf("[LOG] DS1307 already running\r\n");
        }
    }
    else
    {
        printf("[LOG] DS1307 read failed\r\n");
    }

    /* Create the queues */
    s_data_queue  = osMessageQueueNew(LOG_DATA_QUEUE_DEPTH,
                                      sizeof(MonitorData_t),
                                      NULL);
    s_event_queue = osMessageQueueNew(LOG_EVENT_QUEUE_DEPTH,
                                      sizeof(CommEventPayload_t),
                                      NULL);
}

osMessageQueueId_t Log_GetDataQueueHandle(void)
{
    return s_data_queue;
}

osMessageQueueId_t Log_GetEventQueueHandle(void)
{
    return s_event_queue;
}

void Log_WriteData(const MonitorData_t *data)
{
    char date_str[9];
    char filename[25];
    char line[64];

    log_get_date_str(date_str);
    log_build_filename(filename, LOG_DATA_PREFIX, date_str);

    (void)sprintf(line, "%lu,%d,%u,%u,%u,%u\r\n",
                  (unsigned long)(HAL_GetTick() / 1000U),
                  (int)data->temperature,
                  (unsigned)data->humidity,
                  (unsigned)data->battery,
                  (unsigned)data->light,
                  (unsigned)data->system_mode);

    log_enforce_retention(LOG_DATA_PREFIX);
    (void)log_append_line(filename, line,
                      "timestamp_s,temp_c,humidity_pct,battery_raw,light_raw,mode\r\n");
}

void Log_WriteEvent(const CommEventPayload_t *event)
{
    char date_str[9];
    char filename[25];
    char line[48];

    log_get_date_str(date_str);
    log_build_filename(filename, LOG_EVNT_PREFIX, date_str);

    (void)sprintf(line, "%lu,%u,%u\r\n",
                  (unsigned long)event->timestamp,
                  (unsigned)event->event_type,
                  (unsigned)event->detail);

    log_enforce_retention(LOG_EVNT_PREFIX);
    (void)log_append_line(filename, line,
                      "timestamp_s,event_type,detail\r\n");
}

void Log_Task(void *argument)
{
    MonitorData_t       data;
    CommEventPayload_t  event;
    osStatus_t          status;

    (void)argument;
    osDelay(10000U);
    printf("[LOG] task started\r\n");

    for (;;)
    {
        status = osMessageQueueGet(s_data_queue, &data, NULL, 0U);
        if (osOK == status)
        {
            Log_WriteData(&data);
        }

        status = osMessageQueueGet(s_event_queue, &event, NULL, 0U);
        if (osOK == status)
        {
            Log_WriteEvent(&event);
        }

        osDelay(100U);
    }
}