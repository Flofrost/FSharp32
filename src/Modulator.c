#include "Modulator.h"
#include "Pinout.h"

volatile char incrementsModulator = 0;

ISR(INT0_vect){
    if(PHASE_B) incrementsModulator++;
    else        incrementsModulator--;
}

ISR(INT1_vect){
    if(PHASE_A) incrementsModulator--;
    else        incrementsModulator++;
}