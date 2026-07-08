#ifndef DATETIME_H
#define DATETIME_H

#include <stdint.h>

typedef struct
{
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t weekday;
    uint8_t hours;
    uint8_t minutes;
    uint8_t seconds;
} DateTime;

void DateTime_Init(void);
void DateTime_Poll(DateTime *dt);
void DateTime_Print(const DateTime *dt);
uint8_t DateTime_SetDate(uint8_t year, uint8_t month, uint8_t day, uint8_t weekday);
uint8_t DateTime_SetTime(uint8_t hours, uint8_t minutes, uint8_t seconds);

#endif /* DATETIME_H */