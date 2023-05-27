#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include <avr/pgmspace.h>

typedef struct Instrument{
    const int8_t samples[256];
    const int8_t name[16];
    const int8_t icon;
}Instrument;

#define N_INSTRUMENTS 6
extern const Instrument instrumentList[N_INSTRUMENTS] PROGMEM;

extern const int8_t instrumentSine[256] PROGMEM;

extern const uint16_t noteFrequencies[4][32] PROGMEM;
extern const int8_t noteNames[4][32][4] PROGMEM;
extern const int8_t frequencyStrings[4][32][5] PROGMEM;

extern int8_t loadedInstrument[256];
#define loadInstrument(instrumentIndex) for(uint16_t i = 0 ; i < 256 ; i++) loadedInstrument[i] = pgm_read_byte(&instrumentList[instrumentIndex].samples[i])

#endif