#ifndef MELODY_H
#define MELODY_H

#include <stdint.h>

/* ---- octave 1 (Hz) ---- */
#define C1   262
#define Cs1  277
#define D1   294
#define Ds1  311
#define E1   330
#define F1   349
#define Fs1  370
#define G1   392
#define Gs1  415
#define A1   440
#define As1  466
#define B1   494

/* ---- octave 2 (Hz) ---- */
#define C2   523
#define Cs2  554
#define D2   587
#define Ds2  622
#define E2   659
#define F2   698
#define Fs2  740
#define G2   784
#define Gs2  831
#define A2   880
#define As2  932
#define B2   988

#define BEAT_MS  400
#define GAP_MS   40

/* derives length from the array itself — swap arrays with zero other edits */
#define NOTE_COUNT(arr)  (sizeof(arr) / sizeof((arr)[0]))

typedef struct
{
    uint16_t frequency;  /* Hz; 0 = rest */
    uint16_t beats;      /* 1=quarter 2=half 4=whole */
} Note_t;

void MelodyTask(void *argument);

#endif /* MELODY_H */