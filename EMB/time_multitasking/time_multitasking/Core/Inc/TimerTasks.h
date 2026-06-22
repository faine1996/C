#ifndef TIMER_TASKS_H
#define TIMER_TASKS_H

#include <stdint.h>

typedef void (* TimerCallbackHandler)(void *);

typedef struct TimerTasks_ TimerTasks;

TimerTasks * TimerTasks_create(void);

void TimerTasks_registerCallback(
    TimerTasks * t,
    TimerCallbackHandler func,
    void * obj,
    uint32_t periodic
);

void TimerTasks_interrupt(TimerTasks * t);

#endif