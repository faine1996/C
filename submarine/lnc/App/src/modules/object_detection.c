#include "object_detection.h"
#include "ir_receiver.h"

/* Last known IR state — 1 if object was detected on previous poll,
 * 0 if not. Initialised by ObjDet_Init(). */
static uint8_t s_prev_detected;

void ObjDet_Init(void)
{
    /* Snapshot current IR state as baseline so the first poll does not
     * produce a spurious event for a pre-existing detection. */
    s_prev_detected = Ir_IsObjectDetected();
}

void ObjDet_Poll(ObjDetEvent_t *event_out)
{
    uint8_t current_detected;

    current_detected = Ir_IsObjectDetected();

    if (current_detected && !s_prev_detected)
    {
        *event_out = OBJDET_DETECTED;
    }
    else if (!current_detected && s_prev_detected)
    {
        *event_out = OBJDET_CLEARED;
    }
    else
    {
        *event_out = OBJDET_NONE;
    }

    s_prev_detected = current_detected;
}

void ObjDet_Reset(void)
{
    s_prev_detected = 0U;
}