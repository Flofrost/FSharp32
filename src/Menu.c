#include "Menu.h"

uint8_t menuButtonPrevious = 0, backButtonPrevious = 0, menuIndex = 0;

void (*screenControlFunction)() = mainScreenController;


void mainScreenInit(){
    cli();
    screenControlFunction = mainScreenController;

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

    sei();
}

void mainScreenController(){
    static uint8_t previousOctave = 255;

    if(octave != previousOctave){
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
        
        previousOctave = octave;
    }
    
    uint8_t voiceIndex = 255;
    for(uint8_t i = 0 ; i < N_VOICES ; i++)
        if(voices[i].stage != off){
            voiceIndex = i;
            break;
        }

    if(voiceIndex != 255){
        int8_t frequencyString[5], noteString[4];

        frequencyString[0] = pgm_read_byte(&frequencyStrings[octave][voices[voiceIndex].originatorKey][0]);
        noteString[0]      = pgm_read_byte(&noteNames       [octave][voices[voiceIndex].originatorKey][0]);
        frequencyString[1] = pgm_read_byte(&frequencyStrings[octave][voices[voiceIndex].originatorKey][1]);
        noteString[1]      = pgm_read_byte(&noteNames       [octave][voices[voiceIndex].originatorKey][1]);
        frequencyString[2] = pgm_read_byte(&frequencyStrings[octave][voices[voiceIndex].originatorKey][2]);
        noteString[2]      = pgm_read_byte(&noteNames       [octave][voices[voiceIndex].originatorKey][2]);
        frequencyString[3] = pgm_read_byte(&frequencyStrings[octave][voices[voiceIndex].originatorKey][3]);
        noteString[3]      = pgm_read_byte(&noteNames       [octave][voices[voiceIndex].originatorKey][3]);
        frequencyString[4] = pgm_read_byte(&frequencyStrings[octave][voices[voiceIndex].originatorKey][4]);

        printStr_SSD1306(0, 1, frequencyString);
        printStr_SSD1306(0, 2, noteString);
    }else{
        printStr_SSD1306(0, 1, "    ");
        printStr_SSD1306(0, 2, "   ");
    }
    
    if(menuButton != menuButtonPrevious){
        if(menuButton) mainMenuInit();
        menuButtonPrevious = menuButton;
    }
}


void mainMenuInit(){
    cli();
    screenControlFunction = mainMenuController;

    clear_SSD1306();
    printStr_SSD1306(0, 0, "Main Menu");
    sei();
}

void mainMenuController(){
    if(backButton != backButtonPrevious){
        if(backButton) mainScreenInit();
        backButtonPrevious = backButton;
    }
}
