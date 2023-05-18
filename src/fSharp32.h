#ifndef FSHARP32_H
#define FSHARP32_H

#include <avr/eeprom.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include "Inputs.h"
#include "Menu.h"
#include "SSD1306.h"
#include "Instruments.h"

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
    uint8_t  originatorKey;
    NoteStage stage;
} Voice;



extern uint8_t octave;
extern uint8_t EEMEM selectedInstrument;

extern uint8_t keyToVoiceMap[32];
extern volatile Voice voices[N_VOICES];


uint8_t allocateVoice();
void    freeVoice(uint8_t voiceAddress);


#endif