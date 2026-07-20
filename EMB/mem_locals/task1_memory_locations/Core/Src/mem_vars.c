#include "mem_vars.h"
#include <string.h>

extern uint8_t _sram2_data_lma;
extern uint8_t _sram2_data_vma_start;
extern uint8_t _sram2_data_vma_end;

uint32_t g_globalVar = 0x11111111UL;

static uint32_t s_fileStaticVar = 0x22222222UL;

__attribute__((section(".ram2_data"))) uint32_t g_ram2Var = 0x44444444UL;

void mem_vars_init(void)
{
    uint32_t size;

    size = (uint32_t)(&_sram2_data_vma_end - &_sram2_data_vma_start);
    (void)memcpy(&_sram2_data_vma_start, &_sram2_data_lma, size);
}

uint32_t mem_vars_get_ram2_address(void)
{
    return (uint32_t)&g_ram2Var;
}

uint32_t mem_vars_get_file_static_address(void)
{
    return (uint32_t)&s_fileStaticVar;
}