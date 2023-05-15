#include "Voices.h"

uint8_t busyVoices = 0;

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
