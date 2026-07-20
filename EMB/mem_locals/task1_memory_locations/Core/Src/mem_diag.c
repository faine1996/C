#include "mem_diag.h"
#include <stdio.h>

#define FLASH_BASE_ADDR 0x08000000UL
#define FLASH_SIZE 0x00100000UL

#define SRAM1_BASE_ADDR 0x20000000UL
#define SRAM1_SIZE 0x00018000UL

#define SRAM2_BASE_ADDR 0x10000000UL
#define SRAM2_SIZE 0x00008000UL

#define SRAM2_ALIAS_BASE 0x20018000UL
#define SRAM2_ALIAS_SIZE 0x00008000UL

static const char *regionOf(uint32_t addr)
{
    const char *region;

    if ((FLASH_BASE_ADDR <= addr) && (addr < (FLASH_BASE_ADDR + FLASH_SIZE)))
    {
        region = "FLASH";
    }
    else if ((SRAM1_BASE_ADDR <= addr) &&
             (addr < (SRAM1_BASE_ADDR + SRAM1_SIZE)))
    {
        region = "SRAM1";
    }
    else if ((SRAM2_BASE_ADDR <= addr) &&
             (addr < (SRAM2_BASE_ADDR + SRAM2_SIZE)))
    {
        region = "SRAM2";
    }
    else if ((SRAM2_ALIAS_BASE <= addr) &&
             (addr < (SRAM2_ALIAS_BASE + SRAM2_ALIAS_SIZE)))
    {
        region = "SRAM2_ALIAS";
    }
    else
    {
        region = "UNKNOWN";
    }

    return region;
}

void printAddress(const char *message, uint32_t addr)
{
    printf("%s: 0x%08lX (%s)\r\n", message, (unsigned long)addr,
           regionOf(addr));
}