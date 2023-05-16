#ifndef FSHARP32_H
#define FSHARP32_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include "Instruments.h"
#include "Inputs.h"
#include "Voices.h"
#include "SSD1306.h"


#define N_KEYS 32


volatile int8_t vibrato = 0, tremolo = 0, octave = 0;

uint8_t keyToVoiceMap[32];
volatile Voice voices[N_VOICES];

uint32_t keyboardState = 0, keyboardPreviousState = 0;


void (*keyboardHandlingFunction)();

void normalKeyboardOperation();
void toggleKeyboardOperation();
void burstKeyboardOperation();

#endif