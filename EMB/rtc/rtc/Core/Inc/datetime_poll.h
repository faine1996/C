#ifndef DATETIME_POLL_H
#define DATETIME_POLL_H

#include "datetime.h"

extern DateTime g_dateTime;

void Task_DateTimePoll(void *argument);

#endif /* DATETIME_POLL_H */