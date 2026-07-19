#ifndef FLASH_STORAGE_H
#define FLASH_STORAGE_H

#include "stm32l4xx_hal.h"

#define FLASH_STORAGE_MSG_LEN   ((uint32_t)32U)

uint8_t FlashStorage_IsWritten(void);
void FlashStorage_Write(void);
void FlashStorage_Read(char *buffer, uint32_t len);

#endif /* FLASH_STORAGE_H */