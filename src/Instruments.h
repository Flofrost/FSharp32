#ifndef INSTRUMENTS_H
#define INSTRUMENTS_H

#include <avr/pgmspace.h>


extern const int8_t instrumentSine[256] PROGMEM;
extern const uint16_t noteFrequencies[4][32] PROGMEM;
extern const int8_t noteNames[4][32][4] PROGMEM;
extern const int8_t frequencyStrings[4][32][5] PROGMEM;

extern int8_t loadedInstrument[256];
#define loadInstrument(newInstrument) for(uint16_t i = 0 ; i < 256 ; i++) loadedInstrument[i] = pgm_read_byte((newInstrument) + i)

#endif