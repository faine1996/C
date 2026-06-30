#ifndef PRINT_TASKS_H
#define PRINT_TASKS_H

#include "cmsis_os.h"

extern osMutexId_t print_mutex;

void PrintTask(void *argument);
void PrintTasks_Init(void);

#endif /* PRINT_TASKS_H */