#include "Modulator.h"

volatile char increments = 0;

ISR(INT0_vect){
    if(PIND & 0x08) increments--;
    else            increments++;
}

ISR(INT1_vect){
    if(PIND & 0x04) increments++;
    else            increments--;
}