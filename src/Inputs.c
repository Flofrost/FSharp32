#include "Inputs.h"
#include "SSD1306.h"

volatile int8_t incrementsModulator = 0;
volatile uint32_t keyboardState = 0;
uint8_t readingOffset = 0;

ISR(INT0_vect){
    if(PHASE_B) incrementsModulator++;
    else        incrementsModulator--;
}

ISR(INT1_vect){
    if(PHASE_A) incrementsModulator--;
    else        incrementsModulator++;
}


void readKeyboard(){
    uint8_t keys = ~PINB;

    switch(readingOffset){
        case 0:
            keyboardState &= 0xFFFFFF00;
            keyboardState |= (uint32_t)keys;
            break;
        case 1:
            keyboardState &= 0xFFFF00FF;
            keyboardState |= (uint32_t)keys << 8;
            break;
        case 2:
            keyboardState &= 0xFF00FFFF;
            keyboardState |= (uint32_t)keys << 16;
            break;
        case 3:
            keyboardState &= 0x00FFFFFF;
            keyboardState |= (uint32_t)keys << 24;
            break;
    }

    readingOffset++;
    readingOffset &= 0x03;
    DDRD = 0x10 << readingOffset;
}
