#include "flash_storage.h"
#include <string.h>

#define FLASH_STORAGE_PAGE ((uint32_t)255U)
#define FLASH_STORAGE_BANK2_BASE (FLASH_BASE + FLASH_BANK_SIZE)
#define FLASH_STORAGE_PAGE_ADDR                                                \
    (FLASH_STORAGE_BANK2_BASE + (FLASH_STORAGE_PAGE * FLASH_PAGE_SIZE))
#define FLASH_STORAGE_ERASED_DWORD ((uint64_t)0xFFFFFFFFFFFFFFFFU)
#define FLASH_STORAGE_DWORD_BYTES ((uint32_t)8U)

static const char message[FLASH_STORAGE_MSG_LEN] =
    "STM32L476RG Flash Storage OK!!";

uint8_t FlashStorage_IsWritten(void)
{
    uint64_t first_double_word;

    first_double_word = *(volatile uint64_t *)FLASH_STORAGE_PAGE_ADDR;

    if (FLASH_STORAGE_ERASED_DWORD == first_double_word)
    {
        return (uint8_t)0U;
    }

    return (uint8_t)1U;
}

void FlashStorage_Write(void)
{
    FLASH_EraseInitTypeDef erase_init;
    uint32_t page_error;
    uint32_t index;
    uint32_t address;
    uint64_t double_word;

    HAL_FLASH_Unlock();

    erase_init.TypeErase = FLASH_TYPEERASE_PAGES;
    erase_init.Banks = FLASH_BANK_2;
    erase_init.Page = FLASH_STORAGE_PAGE;
    erase_init.NbPages = (uint32_t)1U;

    HAL_FLASHEx_Erase(&erase_init, &page_error);

    address = FLASH_STORAGE_PAGE_ADDR;

    for (index = (uint32_t)0U;
         index < (FLASH_STORAGE_MSG_LEN / FLASH_STORAGE_DWORD_BYTES); index++)
    {
        memcpy(&double_word, &message[index * FLASH_STORAGE_DWORD_BYTES],
               FLASH_STORAGE_DWORD_BYTES);
        HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, double_word);
        address += FLASH_STORAGE_DWORD_BYTES;
    }

    HAL_FLASH_Lock();
}

void FlashStorage_Read(char *buffer, uint32_t len)
{
    const char *source;
    uint32_t index;

    source = (const char *)FLASH_STORAGE_PAGE_ADDR;

    for (index = (uint32_t)0U; index < len; index++)
    {
        buffer[index] = source[index];
    }
}