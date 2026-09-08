#ifndef TIMEUTIL_H
#define TIMEUTIL_H

#include <stdint.h>

/**
 * @brief   Converts a civil date and time to Unix epoch seconds.
 * @param   yy  Year, 2 digits, offset from 2000 (26 = 2026).
 * @param   mm  Month, 1-12.
 * @param   dd  Day of month, 1-31.
 * @param   hh  Hour, 0-23.
 * @param   mi  Minute, 0-59.
 * @param   ss  Second, 0-59.
 * @retval  Unix epoch seconds (seconds since 1970-01-01 00:00:00 UTC).
 */
uint32_t TimeUtil_ToEpoch(uint8_t yy,
                          uint8_t mm,
                          uint8_t dd,
                          uint8_t hh,
                          uint8_t mi,
                          uint8_t ss);

/**
 * @brief   Converts Unix epoch seconds to a civil date (year/month/day
 *          only — no time-of-day breakdown, since the only caller needs
 *          to enumerate which calendar dates a range spans).
 * @param   epoch  Unix epoch seconds.
 * @param   yy     Output: year, 2 digits, offset from 2000.
 * @param   mm     Output: month, 1-12.
 * @param   dd     Output: day of month, 1-31.
 * @retval  None.
 */
void TimeUtil_FromEpoch(uint32_t epoch,
                        uint8_t *yy,
                        uint8_t *mm,
                        uint8_t *dd);

#endif /* TIMEUTIL_H */
