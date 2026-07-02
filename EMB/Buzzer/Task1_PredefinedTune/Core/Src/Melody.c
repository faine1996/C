#include "Melody.h"
#include "Buzzer.h"
#include "FreeRTOS.h"
#include "task.h"
#include <stdio.h>
#include <string.h>

extern UART_HandleTypeDef huart2;

static const uint16_t note_freqs[] = {262, 277, 294, 311, 330, 349, 370, 392,
                                      415, 440, 466, 494, 523, 554, 587, 622,
                                      659, 698, 740, 784, 831, 880, 932, 988};
static const char *const note_names[] = {
    "C1 ", "C#1", "D1 ", "D#1", "E1 ", "F1 ", "F#1", "G1 ",
    "G#1", "A1 ", "A#1", "B1 ", "C2 ", "C#2", "D2 ", "D#2",
    "E2 ", "F2 ", "F#2", "G2 ", "G#2", "A2 ", "A#2", "B2 "};

static char s_tx_buf[40];

static const char *freq_to_name(uint16_t freq)
{
    uint32_t i;
    for (i = 0; NOTE_COUNT(note_freqs) > i; i++)
    {
        if (freq == note_freqs[i])
        {
            return note_names[i];
        }
    }
    return "---";
}

static void print_note(uint16_t freq, uint16_t beats)
{
    const char *name;
    int len;

    name = (0 == freq) ? "REST" : freq_to_name(freq);
    len = sprintf(s_tx_buf, "%-4s  %3u Hz  %u beat(s)\r\n", name,
                  (unsigned)freq, (unsigned)beats);
    HAL_UART_Transmit(&huart2, (uint8_t *)s_tx_buf, (uint16_t)len, 100);
}

/* replace with the course-provided Yonatan sequence */
static const Note_t yonatan[] = {
    {C1, 1}, {C1, 1}, {G1, 1}, {G1, 1}, {A1, 1}, {A1, 1}, {G1, 2},
    {F1, 1}, {F1, 1}, {E1, 1}, {E1, 1}, {D1, 1}, {D1, 1}, {C1, 2},
    {G1, 1}, {G1, 1}, {F1, 1}, {F1, 1}, {E1, 1}, {E1, 1}, {D1, 2},
    {G1, 1}, {G1, 1}, {F1, 1}, {F1, 1}, {E1, 1}, {E1, 1}, {D1, 2},
    {C1, 1}, {C1, 1}, {G1, 1}, {G1, 1}, {A1, 1}, {A1, 1}, {G1, 2},
    {F1, 1}, {F1, 1}, {E1, 1}, {E1, 1}, {D1, 1}, {D1, 1}, {C1, 4}};

void MelodyTask(void *argument)
{
    uint32_t i;

    (void)argument;

    for (i = 0; NOTE_COUNT(yonatan) > i; i++)
    {
        print_note(yonatan[i].frequency, yonatan[i].beats);
        Buzzer_PlayNote(yonatan[i].frequency,
                        (uint32_t)yonatan[i].beats * BEAT_MS - GAP_MS);
        vTaskDelay(pdMS_TO_TICKS((uint32_t)yonatan[i].beats * BEAT_MS));
    }

    vTaskDelete(NULL);
}