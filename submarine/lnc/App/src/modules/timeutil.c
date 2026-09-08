#include "timeutil.h"

#define SECONDS_PER_DAY 86400UL

/*
 * Converts a civil date (full year, month 1-12, day 1-31) to the number
 * of days since 1970-01-01. Proleptic Gregorian calendar algorithm from
 * Howard Hinnant's "days_from_civil" — correct for all valid dates,
 * including leap years, with no floating point and no <time.h> support
 * (this project has none on the embedded side).
 */
static int32_t days_from_civil(int32_t y, uint8_t m, uint8_t d)
{
    int32_t era;
    int32_t yoe;
    int32_t doy;
    int32_t doe;

    y -= (2U >= m) ? 1 : 0;
    era = (0 <= y ? y : y - 399) / 400;
    yoe = y - era * 400;
    doy = (153 * ((int32_t)m + ((2U < m) ? -3 : 9)) + 2) / 5 + (int32_t)d - 1;
    doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;

    return era * 146097 + doe - 719468;
}

/*
 * Inverse of days_from_civil: converts a day count since 1970-01-01 back
 * into a civil date (full year, month 1-12, day 1-31).
 */
static void civil_from_days(int32_t z, int32_t *y, uint8_t *m, uint8_t *d)
{
    int32_t era;
    int32_t doe;
    int32_t yoe;
    int32_t doy;
    int32_t mp;

    z  += 719468;
    era = (0 <= z ? z : z - 146096) / 146097;
    doe = z - era * 146097;
    yoe = (doe - doe / 1460 + doe / 36524 - doe / 146096) / 365;
    *y  = yoe + era * 400;
    doy = doe - (365 * yoe + yoe / 4 - yoe / 100);
    mp  = (5 * doy + 2) / 153;
    *d  = (uint8_t)(doy - (153 * mp + 2) / 5 + 1);
    *m  = (uint8_t)((10 > mp) ? (mp + 3) : (mp - 9));

    if (2U >= *m)
    {
        ++(*y);
    }
}

uint32_t TimeUtil_ToEpoch(uint8_t yy,
                          uint8_t mm,
                          uint8_t dd,
                          uint8_t hh,
                          uint8_t mi,
                          uint8_t ss)
{
    int32_t  days;
    uint32_t seconds;

    days = days_from_civil((int32_t)yy + 2000, mm, dd);

    seconds = (uint32_t)days * SECONDS_PER_DAY
            + (uint32_t)hh * 3600UL
            + (uint32_t)mi * 60UL
            + (uint32_t)ss;

    return seconds;
}

void TimeUtil_FromEpoch(uint32_t epoch,
                        uint8_t *yy,
                        uint8_t *mm,
                        uint8_t *dd)
{
    int32_t days;
    int32_t year;

    days = (int32_t)(epoch / SECONDS_PER_DAY);

    civil_from_days(days, &year, mm, dd);

    *yy = (uint8_t)(year - 2000);
}
