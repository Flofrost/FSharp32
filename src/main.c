#include <avr/io.h>
#include <avr/interrupt.h>
#include "Instruments.h"
#include "SSD1306.h"
#include "Modulator.h"

volatile unsigned char time = 0;

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
    
    time++;
}

// thing
ISR(TIMER1_OVF_vect){      // ~ 30 Hz
    sei();
    // printStr_SSD1306(0, 0, "      ");
    // printInt_SSD1306(0, 0, time);
    // printStr_SSD1306(0, 1, "      ");
    // printInt_SSD1306(0, 1, speedModulator);
    vibrato >>= 1;
    vibrato += incrementsModulator;
    incrementsModulator = 0;
}


int main(){
    
    TCCR1B = 0x02; // interrput at rougly 30 Hz
    TCCR2 = 0x0A;
    OCR2  = 127;
    TIMSK = 0x84; // output sample at roughly 15 kHz
    
    MCUCR = 0x0A;
    GICR  = 0xC0; // Enable INT0 and INT1

    UCSRB = 0x18;
    UBRRL = 0x08;
    
    DDRA = 0xFF;
    
    changeInstrument(sinValues);

    init_SSD1306();
    clear_SSD1306();

    sei();
    
    while(1);
                  
    return 0;
}