#ifndef DS1307_H
#define DS1307_H

#include <stdint.h>

#define DS1307_I2C_ADDRESS (0xD0U)

typedef struct
{
    uint8_t seconds;
    uint8_t minutes;
    uint8_t hours;
    uint8_t day_of_week;
    uint8_t date;
    uint8_t month;
    uint8_t year;
} Ds1307_Time_t;

typedef enum
{
    DS1307_OK,
    DS1307_ERROR
} Ds1307_Status_t;

Ds1307_Status_t Ds1307_SetTime(const Ds1307_Time_t *time);
Ds1307_Status_t Ds1307_GetTime(Ds1307_Time_t *time);
Ds1307_Status_t Ds1307_IsTimeSet(uint8_t *is_set);

#endif /* DS1307_H */