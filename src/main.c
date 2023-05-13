#include <avr/interrupt.h>
#include "Inputs.h"
#include "Instruments.h"
#include "SSD1306.h"

volatile int8_t vibrato = 0, tremolo = 0;

volatile Voice voice = {
    .phase = 0,
    .frequency = 231,
    .amplitude = 0x40
};

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
    int16_t sample = activeInstrument[voice.phase >> 8] >> 3;
    sample *= voice.amplitude + (tremolo << 2);
    sample >>= 6;
    PORTA = 127 + sample;
    voice.phase += voice.frequency + (vibrato << 5);
}

// thing
ISR(TIMER1_OVF_vect){      // ~ 30 Hz
    sei();

    if(VIBRATO){
        vibrato >>= 1;
        vibrato += incrementsModulator;
        incrementsModulator = 0;
        printStr_SSD1306(0, 7, "FM");
    }else if(TREMOLO){
        tremolo >>= 1;
        tremolo += incrementsModulator;
        incrementsModulator = 0;
        printStr_SSD1306(0, 7, "AM");
    }else printStr_SSD1306(0, 7, "  ");
    
    switch(OCTAVE){
        case 0:
            printStr_SSD1306(9, 0, "12");
            break;
        case 1:
            printStr_SSD1306(9, 0, "34");
            break;
        case 2:
            printStr_SSD1306(9, 0, "56");
            break;
        case 3:
            printStr_SSD1306(9, 0, "78");
            break;
    }
    
    voice.frequency = 231 << (OCTAVE << 1);

    readKeyboard();
    printHex32_SSD1306(0, 1, keyboardState);
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
        
    TWBR = 16;
    
    DDRA = 0xFF; // Output mode for DAC
    PORTC = 0xFC; // Pullups on most of port c for buttons and switches
    PORTB = 0xFF; // Pullups for key matrix
    
    changeInstrument(sinValues);

    init_SSD1306();
    clear_SSD1306();
    printStr_SSD1306(0, 0, "OCTAVE : ");

    sei();
    
    while(1);
                  
    return 0;
}