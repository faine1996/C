#include "TimerTasks.h"
#include <stdlib.h>

typedef struct
{
    TimerCallbackHandler handler;
    void *param;
    uint32_t periodic;
    uint32_t counter;
} TimerTask;

struct TimerTasks_
{
    TimerTask tasks[10];
    uint32_t task_count;
};

TimerTasks * TimerTasks_create(void)
{
    TimerTasks *t;
    uint32_t i;

    t = (TimerTasks *)malloc(sizeof(TimerTasks));
    if (t == 0)
    {
        return 0;
    }

    t->task_count = 0;
    for (i = 0; i < 10; i = i + 1)
    {
        t->tasks[i].handler = 0;
        t->tasks[i].param = 0;
        t->tasks[i].periodic = 0;
        t->tasks[i].counter = 0;
    }

    return t;
}

void TimerTasks_registerCallback(
    TimerTasks *t,
    TimerCallbackHandler func,
    void *obj,
    uint32_t periodic
)
{
    uint32_t idx;

    if (t->task_count >= 10)
    {
        return;
    }

    idx = t->task_count;
    t->tasks[idx].handler = func;
    t->tasks[idx].param = obj;
    t->tasks[idx].periodic = periodic / 10;
    t->tasks[idx].counter = 0;
    t->task_count = t->task_count + 1;
}

void TimerTasks_interrupt(TimerTasks *t)
{
    uint32_t i;

    for (i = 0; i < t->task_count; i = i + 1)
    {
        t->tasks[i].counter = t->tasks[i].counter + 1;

        if (t->tasks[i].counter == t->tasks[i].periodic)
        {
            t->tasks[i].handler(t->tasks[i].param);
            t->tasks[i].counter = 0;
        }
    }
}
