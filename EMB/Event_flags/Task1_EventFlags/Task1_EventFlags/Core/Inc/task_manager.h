#ifndef TASK_MANAGER_H
#define TASK_MANAGER_H

#include "cmsis_os.h"

#define FLAG_START500   (1U << 0)
#define FLAG_START700   (1U << 1)
#define FLAG_START1100  (1U << 2)
#define FLAG_DONE500    (1U << 3)
#define FLAG_DONE700    (1U << 4)
#define FLAG_DONE1100   (1U << 5)

#define FLAG_ALL_START  (FLAG_START500  | FLAG_START700  | FLAG_START1100)
#define FLAG_ALL_DONE   (FLAG_DONE500   | FLAG_DONE700   | FLAG_DONE1100)

extern osEventFlagsId_t xEventGroup;

#endif /* TASK_MANAGER_H */