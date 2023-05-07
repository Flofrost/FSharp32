#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include "SSD1306.h"

#define changeInstrument(newInstrument) for(unsigned short i = 0 ; i < 256 ; i++) activeInstrument[i] = pgm_read_byte((newInstrument) + i)

const char sinValues[256] PROGMEM = {0, 3, 6, 9, 12, 15, 18, 21, 24, 28, 31, 34, 37, 40, 43, 46, 48, 51, 54, 57, 60, 63, 65, 68, 71, 73, 76, 78, 81, 83, 85, 88, 90, 92, 94, 96, 98, 100, 102, 104, 106, 108, 109, 111, 112, 114, 115, 117, 118, 119, 120, 121, 122, 123, 124, 124, 125, 126, 126, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 127, 126, 126, 125, 124, 124, 123, 122, 121, 120, 119, 118, 117, 115, 114, 112, 111, 109, 108, 106, 104, 102, 100, 98, 96, 94, 92, 90, 88, 85, 83, 81, 78, 76, 73, 71, 68, 65, 63, 60, 57, 54, 51, 48, 46, 43, 40, 37, 34, 31, 28, 24, 21, 18, 15, 12, 9, 6, 3, 0, -3, -6, -9, -12, -15, -18, -21, -24, -28, -31, -34, -37, -40, -43, -46, -48, -51, -54, -57, -60, -63, -65, -68, -71, -73, -76, -78, -81, -83, -85, -88, -90, -92, -94, -96, -98, -100, -102, -104, -106, -108, -109, -111, -112, -114, -115, -117, -118, -119, -120, -121, -122, -123, -124, -124, -125, -126, -126, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -127, -126, -126, -125, -124, -124, -123, -122, -121, -120, -119, -118, -117, -115, -114, -112, -111, -109, -108, -106, -104, -102, -100, -98, -96, -94, -92, -90, -88, -85, -83, -81, -78, -76, -73, -71, -68, -65, -63, -60, -57, -54, -51, -48, -46, -43, -40, -37, -34, -31, -28, -24, -21, -18, -15, -12, -9, -6, -3};
char activeInstrument[256] = {0};
volatile unsigned long timeCounter = 0;
volatile unsigned short frequency = 262;

ISR(TIMER0_COMP_vect){
    PORTA = 127 + activeInstrument[(unsigned char)((timeCounter * frequency) >> 4)];
    // PORTA = timeCounter * frequency >> 4;
    timeCounter++;
}

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

void main(){
    
    TCCR0 = 0x0A;
    OCR0  = 127;
    TIMSK = 0x02; // output sample at roughly 15 kHz

    // UCSRB = 0x18;
    // UBRRL = 0x08;
    
    TWBR = 64;

    DDRA = 0xFF;
    
    changeInstrument(sinValues);

    init_SSD1306();
    clear_SSD1306();
    printChar_SSD1306(0, 0, '"');

    sei();
    
    while(1){
        // uartSendSTR("HELLO\n");
    }
                  
    return;
}