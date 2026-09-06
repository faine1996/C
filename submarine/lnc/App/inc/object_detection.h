#ifndef OBJECT_DETECTION_H
#define OBJECT_DETECTION_H

#include <stdint.h>

/* -----------------------------------------------------------------------
 * Types
 * --------------------------------------------------------------------- */

typedef enum
{
    OBJDET_NONE     = 0,
    OBJDET_DETECTED = 1,
    OBJDET_CLEARED  = 2
} ObjDetEvent_t;

/* -----------------------------------------------------------------------
 * Public interface
 * --------------------------------------------------------------------- */

/**
 * @brief   Initialises object detection state. Reads the current IR
 *          driver state so that a pre-existing detection on startup does
 *          not fire a spurious OBJDET_DETECTED event on the first poll.
 * @param   None.
 * @retval  None.
 */
void ObjDet_Init(void);

/**
 * @brief   Compares the current IR driver state against the previously
 *          seen state and reports any transition.
 * @param   event_out  Pointer to an ObjDetEvent_t that receives
 *                     OBJDET_DETECTED, OBJDET_CLEARED, or OBJDET_NONE.
 * @retval  None.
 */
void ObjDet_Poll(ObjDetEvent_t *event_out);

/**
 * @brief   Resets the object detection state to not-detected, matching
 *          the IR latch after Ir_Clear() is called. Call whenever the
 *          IR latch is cleared externally so ObjDet_Poll's internal
 *          state stays in sync.
 * @param   None.
 * @retval  None.
 */
void ObjDet_Reset(void);

#endif /* OBJECT_DETECTION_H */