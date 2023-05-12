#include <avr/interrupt.h>
#include "Pinout.h"
#include "Instruments.h"
#include "SSD1306.h"
#include "Modulator.h"

volatile unsigned short phase = 0;
volatile unsigned short frequency = 1845;
volatile char           vibrato = 0;

void uartSendINT8(const uint8_t x){
    while(!(UCSR0A & 0x20));
    UDR0 = x;
}

void uartSendSTR(const char* s){
    for(uint8_t i = 0 ; s[i] ; i++){
        while(!(UCSR0A & 0x20));
        UDR0 = s[i];
    }
}

// SOUND GENERATOR
ISR(TIMER2_COMPA_vect){      // 15625 Hz
    PORTA = 127 + (activeInstrument[phase >> 8] >> 1);
    phase += frequency + (vibrato << 5);
}

// thing
ISR(TIMER1_OVF_vect){      // ~ 30 Hz
    // sei();
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
    printUInt8_SSD1306(0, 2, MCUSR, 0);
}


int main(){
    
    TCCR2A = 0x02;
    TCCR2B = 0x02;
    OCR2A  = 127;
    TIMSK2 = 0x02; // output sample at roughly 15 kHz

    TCCR1B = 0x02; 
    TIMSK1 = 0x01; // interrput at rougly 30 Hz
    
    EICRA = 0x0F;
    EIMSK = 0x03; // Enable INT0 and INT1

    UCSR0B = 0x18;
    UBRR0L = 0x08; // Uart setup 115200 baud
    
    DDRA = 0xFF; // Output mode for DAC
    PORTC = 0xFC; // Pullups on most of port c for buttons and switches
    
    changeInstrument(sinValues);

    init_SSD1306();
    clear_SSD1306();

    sei();
    
    while(1) sei();
                  
    return 0;
}