#include "Modulator.h"

volatile char incrementsModulator = 0;

ISR(INT0_vect){
    if(PIND & 0x08) incrementsModulator--;
    else            incrementsModulator++;
}

ISR(INT1_vect){
    if(PIND & 0x04) incrementsModulator++;
    else            incrementsModulator--;
}