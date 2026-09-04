#ifndef TEST_BENCH_H
#define TEST_BENCH_H

#include <stdint.h>

/**
 * @brief  Records one pass/fail result and prints it over UART.
 * @param  condition  Non-zero for pass, zero for fail.
 * @param  name       Short label for the check, printed in the report.
 */
#define TEST_CHECK(condition, name) \
    TestBench_RecordResult((uint8_t)((condition) != 0), (name))

/**
 * @brief   Resets the pass/fail counters to zero.
 * @param   None.
 * @retval  None.
 */
void TestBench_Init(void);

/**
 * @brief   Prints PASS/FAIL for one check and updates the running tally.
 * @param   passed  Non-zero if the check passed, zero if it failed.
 * @param   name    Short label for the check, printed in the report.
 * @retval  None.
 */
void TestBench_RecordResult(uint8_t passed, const char *name);

/**
 * @brief   Prints the total pass/fail counts accumulated so far.
 * @param   None.
 * @retval  None.
 */
void TestBench_Summary(void);

#endif /* TEST_BENCH_H */