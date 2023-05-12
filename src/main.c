#include <avr/interrupt.h>
#include "Pinout.h"
#include "Instruments.h"
#include "SSD1306.h"
#include "Modulator.h"

volatile unsigned short phase = 0;
volatile unsigned short frequency = 1845;
volatile char           vibrato = 0;

void uartSendINT8(const uint8_t x){
    while(!(UCSRA & 0x20));
    UDR = x;
}

void uartSendSTR(const char* s){
    for(uint8_t i = 0 ; s[i] ; i++){
        while(!(UCSRA & 0x20));
        UDR = s[i];
    }
}

// SOUND GENERATOR
ISR(TIMER2_COMP_vect){      // 15625 Hz
    PORTA = 127 + (activeInstrument[phase >> 8] >> 1);
    phase += frequency + (vibrato << 5);
}

// thing
ISR(TIMER1_OVF_vect){      // ~ 30 Hz
    sei();
    printStr_SSD1306(0, 0, "  ");
    if(VIBRATO){
        printStr_SSD1306(0, 0, "FM");
        vibrato >>= 1;
        vibrato += incrementsModulator;
        incrementsModulator = 0;
    }
    if(TREMOLO){
        printStr_SSD1306(0, 0, "AM");
    }
    printUInt8_SSD1306(0, 1, PINC, 0);
}


int main(){
    
    TCCR1B = 0x02; // interrput at rougly 30 Hz
    TCCR2 = 0x0A;
    OCR2  = 127;
    TIMSK = 0x84; // output sample at roughly 15 kHz
    
    MCUCR = 0x0A;
    GICR  = 0xC0; // Enable INT0 and INT1

    UCSRB = 0x18;
    UBRRL = 0x08; // Uart setup 115200 baud
    
    DDRA = 0xFF; // Output mode for DAC
    PORTC = 0xFC; // Pullups on most of port c for buttons and switches
    
    changeInstrument(sinValues);

    init_SSD1306();
    clear_SSD1306();

    sei();
    
    while(1);
                  
    return 0;
}