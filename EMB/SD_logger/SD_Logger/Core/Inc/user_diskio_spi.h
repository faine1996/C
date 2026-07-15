#ifndef __USER_DISKIO_SPI_H
#define __USER_DISKIO_SPI_H

#ifdef __cplusplus
extern "C" {
#endif

#include "ff_gen_drv.h"

#define HSPI_SDCARD   (&hspi1)
#define SPI_TIMEOUT   100

DSTATUS USER_SPI_initialize (BYTE pdrv);
DSTATUS USER_SPI_status (BYTE pdrv);
DRESULT USER_SPI_read (BYTE pdrv, BYTE *buff, DWORD sector, UINT count);
DRESULT USER_SPI_write (BYTE pdrv, const BYTE *buff, DWORD sector, UINT count);
DRESULT USER_SPI_ioctl (BYTE pdrv, BYTE cmd, void *buff);

#ifdef __cplusplus
}
#endif

#endif /* __USER_DISKIO_SPI_H */