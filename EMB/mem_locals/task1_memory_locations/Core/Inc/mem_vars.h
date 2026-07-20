#ifndef MEM_VARS_H
#define MEM_VARS_H

#include <stdint.h>

extern uint32_t g_globalVar;

void mem_vars_init(void);
uint32_t mem_vars_get_ram2_address(void);
uint32_t mem_vars_get_file_static_address(void);

#endif /* MEM_VARS_H */