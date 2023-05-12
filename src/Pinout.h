#ifndef PINOUT_H
#define PINOUT_H

#include <avr/io.h>

#define PHASE_A PIND & 0x04
#define PHASE_B PIND & 0x08

#define VIBRATO !(PINC & 0x10)
#define TREMOLO !(PINC & 0x20)

#endif