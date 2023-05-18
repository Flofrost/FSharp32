#include "fSharp32.h"
#include "UART.h"


uint8_t octave = 0;
volatile int8_t vibrato = 0, tremolo = 0;
uint8_t busyVoices = 0;

uint8_t keyToVoiceMap[32];
volatile Voice voices[N_VOICES];


uint8_t allocateVoice(){
    for(uint8_t i = 0 ; i < N_VOICES ; i++)
        if(!((busyVoices >> i) & 0x01)){
            busyVoices |= 1 << i;
            return i;
        }
    return 255;
}

void freeVoice(uint8_t voiceAddress){
    if(voiceAddress < N_VOICES) busyVoices &= ~(1 << voiceAddress);
}



// SOUND GENERATOR
ISR(TIMER2_COMPA_vect){ // 15625 Hz
    int8_t sample = 0, voiceSample;

    for(uint8_t i = 0 ; i < N_VOICES ; i++)
        if(voices[i].stage != off){
            voiceSample = loadedInstrument[voices[i].phase >> 8] >> 2;
            voiceSample = (voiceSample * (voices[i].amplitude + (tremolo << 3))) >> 7;
            sample += voiceSample;

            voices[i].phase += voices[i].frequency + (vibrato << 4);
        }

    PORTA = 127 + sample;
}

// Reading Other Inputs and Screen Management
ISR(TIMER0_OVF_vect){ // ~ 61 Hz
    sei();

    octave = OCTAVE;
    menuButton = MENU;
    backButton = BACK;
    
    screenControlFunction();

    if(VIBRATO){
        vibrato >>= 1;
        vibrato += incrementsModulator;
        incrementsModulator = 0;
    }else if(TREMOLO){
        tremolo >>= 1;
        tremolo += incrementsModulator;
        incrementsModulator = 0;
    }
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

    loadInstrument(1);

    keyboardHandlingFunction = normalKeyboardOperation;

    init_SSD1306();
    
    mainScreenInit();
    
    sei();
    
    while(1);
                  
    return 0;
}