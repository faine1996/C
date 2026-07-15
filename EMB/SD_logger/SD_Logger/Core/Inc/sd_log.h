#ifndef __SD_LOG_H
#define __SD_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sensor_types.h"

typedef enum
{
    SD_LOG_OK = 0,
    SD_LOG_ERROR_MOUNT,
    SD_LOG_ERROR_NO_FILESYSTEM,
    SD_LOG_ERROR_OPEN,
    SD_LOG_ERROR_WRITE
}
SdLog_Status_t;

SdLog_Status_t SdLog_Mount(void);
SdLog_Status_t SdLog_AppendReading(const DhtReading_t *reading);

#ifdef __cplusplus
}
#endif

#endif /* __SD_LOG_H */