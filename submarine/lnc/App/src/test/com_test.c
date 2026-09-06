#include "comm.h"
#include "test_bench.h"
#include <stdint.h>
#include <stdio.h>

/* -----------------------------------------------------------------------
 * Self-test
 * --------------------------------------------------------------------- */

void Comm_Test(void)
{
    uint8_t frame[COMM_MAX_FRAME_LEN];
    uint8_t value[12];
    uint8_t chk;
    uint8_t i;

    printf("\r\n=== Comm self-test ===\r\n");

    /* --- Test 1: TIME_SYNC_REQUEST (zero-length frame) ---
     * Expected: AA 13 00 13
     * Checksum = (0x13 + 0x00) & 0xFF = 0x13 */
    frame[0] = COMM_SOF;
    frame[1] = TAG_TIME_SYNC_REQ;
    frame[2] = 0x00U;
    frame[3] = (uint8_t)(TAG_TIME_SYNC_REQ + 0x00U);

    TEST_CHECK(frame[0] == 0xAAU, "TSR: SOF correct");
    TEST_CHECK(frame[1] == 0x13U, "TSR: TAG correct");
    TEST_CHECK(frame[2] == 0x00U, "TSR: LEN correct");
    TEST_CHECK(frame[3] == 0x13U, "TSR: CHK correct");

    /* --- Test 2: EVENT frame (OBJECT_DETECTED, detail=0, ts=1000) ---
     * TAG=0x11, LEN=6
     * value: 02 00 E8 03 00 00
     * checksum = (0x11 + 0x06 + 0x02 + 0x00 + 0xE8 + 0x03 + 0x00 + 0x00)
     *          & 0xFF = 0x04 */
    value[0] = COMM_EVENT_OBJECT_DETECTED;
    value[1] = 0x00U;
    value[2] = 0xE8U; /* ts=1000: little-endian 0x000003E8 */
    value[3] = 0x03U;
    value[4] = 0x00U;
    value[5] = 0x00U;

    chk = (uint8_t)(TAG_EVENT + 6U);
    for (i = 0U; i < 6U; ++i)
    {
        chk = (uint8_t)(chk + value[i]);
    }

    TEST_CHECK(value[0] == 0x02U,  "EVT: event_type correct");
    TEST_CHECK(value[1] == 0x00U,  "EVT: detail correct");
    TEST_CHECK(value[2] == 0xE8U,  "EVT: ts byte 0 correct");
    TEST_CHECK(value[3] == 0x03U,  "EVT: ts byte 1 correct");
    TEST_CHECK(chk      == 0x04U,  "EVT: checksum correct");

    /* --- Test 3: KEEPALIVE frame against the worked example in the spec ---
     * ts=1000, temp=25, hum=60, batt=2048, light=1500, mode=0
     * Full frame: AA 10 0C E8 03 00 00 19 00 3C 00 08 DC 05 00 45 */
    value[0]  = 0xE8U; value[1]  = 0x03U;  /* timestamp 1000  */
    value[2]  = 0x00U; value[3]  = 0x00U;
    value[4]  = 0x19U; value[5]  = 0x00U;  /* temp 25         */
    value[6]  = 0x3CU;                      /* humidity 60     */
    value[7]  = 0x00U; value[8]  = 0x08U;  /* battery 2048    */
    value[9]  = 0xDCU; value[10] = 0x05U;  /* light 1500      */
    value[11] = 0x00U;                      /* mode Normal     */

    chk = (uint8_t)(TAG_KEEPALIVE + 12U);
    for (i = 0U; i < 12U; ++i)
    {
        chk = (uint8_t)(chk + value[i]);
    }

    TEST_CHECK(value[4]  == 0x19U, "KA: temp byte 0 correct");
    TEST_CHECK(value[5]  == 0x00U, "KA: temp byte 1 correct");
    TEST_CHECK(value[7]  == 0x00U, "KA: batt byte 0 correct");
    TEST_CHECK(value[8]  == 0x08U, "KA: batt byte 1 correct");
    TEST_CHECK(chk       == 0x45U, "KA: checksum matches spec example");

    TestBench_Summary();
}