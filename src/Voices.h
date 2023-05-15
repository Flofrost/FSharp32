#ifndef VOICES_H
#define VOICES_H

#include <avr/common.h>


#define N_VOICES 4


typedef enum NoteStage {
    off,
    attack,
    decay,
    sustain,
    release
} NoteStage;

typedef struct Voice{
    uint16_t phase;
    uint16_t frequency;
    uint8_t  amplitude;
    NoteStage stage;
} Voice;


extern volatile Voice voices[N_VOICES];


uint8_t allocateVoice();
void    freeVoice(uint8_t voiceAddress);

#endif