#include <avr/interrupt.h>
#include "Inputs.h"
#include "Instruments.h"
#include "SSD1306.h"

volatile int8_t vibrato = 0, tremolo = 0;

volatile Voice voices[4] = {voiceDefaults, voiceDefaults, voiceDefaults, voiceDefaults};

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
    int8_t sample = 0;
    int16_t voiceSample;

    for(uint8_t i = 0 ; i < 4 ; i++)
        if(voices[0].stage != off){
            voiceSample = activeInstrument[voices[0].phase >> 8] >> 3;
            voiceSample *= voices[0].amplitude + (tremolo << 2);
            sample += voiceSample >> 6;

            voices[0].phase += voices[0].frequency + (vibrato << 3);
        }

    PORTA = 127 + sample;
}

// thing
ISR(TIMER1_COMPA_vect){      // ~ 61 Hz
    static uint32_t keyboardState = 0, keyboardPreviousState = 0;
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

    readKeyboard(&keyboardState);
    if(keyboardState != keyboardPreviousState){
        uint32_t presses = keyboardState ^ keyboardPreviousState;
        uint32_t releases = (~keyboardState) & presses;
        presses = keyboardState & presses;

        if(presses)
            for(uint8_t i = 0 ; i < 32 ; i++)
                if((presses >> i) & 0x01){
                    voices[0].stage = attack;
                    voices[0].frequency = pgm_read_word(&noteFrequencies[OCTAVE][i]);
                    voices[0].amplitude = 0x40;
                }

        if(releases)
            for(uint8_t i = 0 ; i < 32 ; i++)
                if((releases >> i) & 0x01){
                    voices[0].stage = off;
                }

        keyboardPreviousState = keyboardState;
        printHex8_SSD1306(0, 1, OCTAVE);
    }
}


int main(){
    
    TCCR2A = 0x02;
    TCCR2B = 0x02;
    OCR2A  = 127;
    TIMSK2 = 0x02; // output sample at roughly 15 kHz

    TCCR1B = 0x0A; 
    OCR1A  = 0x7FFF;
    TIMSK1 = 0x02; // interrput at rougly 61 Hz
    
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