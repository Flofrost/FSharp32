#include "fSharp32.h"


void normalKeyboardOperation(){
    readKeyboard(&keyboardState);
    if(keyboardState != keyboardPreviousState){
        uint32_t presses = keyboardState ^ keyboardPreviousState;
        uint32_t releases = (~keyboardState) & presses;
        presses = keyboardState & presses;

        if(presses)
            for(uint8_t i = 0 ; i < N_KEYS ; i++)
                if((presses >> i) & 0x01){
                    keyToVoiceMap[i] = allocateVoice();
                    if(keyToVoiceMap[i] != 255){
                        voices[keyToVoiceMap[i]].stage = attack;
                        voices[keyToVoiceMap[i]].frequency = pgm_read_word(&noteFrequencies[octave][i]);
                        voices[keyToVoiceMap[i]].amplitude = 0x80;
                    }
                }

        if(releases)
            for(uint8_t i = 0 ; i < N_KEYS ; i++)
                if((releases >> i) & 0x01){
                    if(keyToVoiceMap[i] != 255){
                        voices[keyToVoiceMap[i]].stage = off;
                        freeVoice(keyToVoiceMap[i]);
                        keyToVoiceMap[i] = 255;
                    }
                }

        keyboardPreviousState = keyboardState;
    }
}

void toggleKeyboardOperation(){
    readKeyboard(&keyboardState);
    if(keyboardState != keyboardPreviousState){
        uint32_t presses = keyboardState ^ keyboardPreviousState;
        uint32_t releases = (~keyboardState) & presses;
        presses = keyboardState & presses;

        if(presses)
            for(uint8_t i = 0 ; i < N_KEYS ; i++)
                if((presses >> i) & 0x01){
                    if(keyToVoiceMap[i] == 255){
                        keyToVoiceMap[i] = allocateVoice();
                        if(keyToVoiceMap[i] != 255){
                            voices[keyToVoiceMap[i]].stage = attack;
                            voices[keyToVoiceMap[i]].frequency = pgm_read_word(&noteFrequencies[octave][i]);
                            voices[keyToVoiceMap[i]].amplitude = 0x80;
                        }
                    }else{
                        voices[keyToVoiceMap[i]].stage = off;
                        freeVoice(keyToVoiceMap[i]);
                        keyToVoiceMap[i] = 255;
                    }
                }

        keyboardPreviousState = keyboardState;
    }
}

void burstKeyboardOperation(){}


void mainScreenDisplayFunction(){
    clear_SSD1306();
    printStr_SSD1306(0, 0, "OCTAVE : ");

    switch(octave){
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

    if(VIBRATO){
        printStr_SSD1306(0, 1, "Modulation : FM");
    }else if(TREMOLO){
        printStr_SSD1306(0, 1, "Modulation : AM");
    }else printStr_SSD1306(0, 1, "               ");
}


// SOUND GENERATOR
ISR(TIMER2_COMPA_vect){ // 15625 Hz
    int8_t sample = 0, voiceSample;

    for(uint8_t i = 0 ; i < N_VOICES ; i++)
        if(voices[i].stage != off){
            voiceSample = activeInstrument[voices[i].phase >> 8] >> 2;
            voiceSample = (voiceSample * (voices[i].amplitude + (tremolo << 3))) >> 7;
            sample += voiceSample;

            voices[i].phase += voices[i].frequency + (vibrato << 4);
        }

    PORTA = 127 + sample;
}

// Reading Keyboard
ISR(TIMER1_OVF_vect){ // ~ 244 Hz
    sei();
    keyboardHandlingFunction();
}

// Reading Other Inputs and Screen Management
ISR(TIMER0_OVF_vect){ // ~ 61 Hz
    sei();

    if(VIBRATO){
        vibrato >>= 1;
        vibrato += incrementsModulator;
        incrementsModulator = 0;
    }else if(TREMOLO){
        tremolo >>= 1;
        tremolo += incrementsModulator;
        incrementsModulator = 0;
    }

    octave = OCTAVE;
    
    screenManager();
}

int main(){
    
    TCCR2A = 0x02;
    TCCR2B = 0x02;
    OCR2A  = 127;
    TIMSK2 = 0x02; // output sample at roughly 15625 Hz

    TCCR1B = 0x01; 
    TIMSK1 = 0x01; // interrput at roughly 244 Hz
    
    TCCR0B = 0x05;
    TIMSK0 = 0x01; // interrupt at roughly 61 Hz
    
    EICRA = 0x0F;
    EIMSK = 0x03; // Enable INT0 and INT1

    UCSR0B = 0x18;
    UBRR0L = 0x08; // Uart setup 115200 baud
        
    TWBR = 16;
    
    DDRA = 0xFF; // Output mode for DAC
    PORTC = 0xFC; // Pullups on most of port c for buttons and switches
    PORTB = 0xFF; // Pullups for key matrix
    
    for(uint8_t i = 0 ; i < N_VOICES ; i++){
        voices[i].phase = 0;
        voices[i].stage = off;
    }

    for(uint8_t i = 0 ; i < N_KEYS ; i++) keyToVoiceMap[i] = 255;

    changeInstrument(sinValues);

    keyboardHandlingFunction = normalKeyboardOperation;

    init_SSD1306();
    clear_SSD1306();

    sei();
    
    while(1);
                  
    return 0;
}