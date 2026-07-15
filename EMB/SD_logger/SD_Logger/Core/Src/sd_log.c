#include "sd_log.h"
#include "ff.h"

static FATFS SdFatFs;
static FIL SdFile;

SdLog_Status_t SdLog_Mount(void)
{
    FRESULT fres;

    fres = f_mount(&SdFatFs, "", 1);

    if (FR_NO_FILESYSTEM == fres)
    {
        return (SD_LOG_ERROR_NO_FILESYSTEM);
    }

    if (FR_OK != fres)
    {
        return (SD_LOG_ERROR_MOUNT);
    }

    return (SD_LOG_OK);
}

SdLog_Status_t SdLog_AppendReading(const DhtReading_t *reading)
{
    SdLog_Status_t status;

    status = SD_LOG_OK;

    if (FR_OK != f_open(&SdFile, "LOG.TXT", FA_OPEN_APPEND | FA_WRITE))
    {
        return (SD_LOG_ERROR_OPEN);
    }

    (void)f_printf(&SdFile, "%lu,%u,%u\r\n", (unsigned long)reading->tick,
                   (unsigned)reading->temperature, (unsigned)reading->humidity);

    if (0 != f_error(&SdFile))
    {
        status = SD_LOG_ERROR_WRITE;
    }

    if (FR_OK != f_close(&SdFile))
    {
        status = SD_LOG_ERROR_WRITE;
    }

    return (status);
}