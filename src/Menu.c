#include "Menu.h"

uint8_t menuButtonPrevious = 0, backButtonPrevious = 0, menuIndex = 0;

void (*screenControlFunction)() = mainScreenController;


const int8_t mainMenuItems[][18] PROGMEM = {
    "Instruments",
    "Keyboard Opt",
    "Envelope"
};
#define N_MAINMENUITEMS (sizeof(mainMenuItems) / sizeof(mainMenuItems[0]))


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
    
    printStr_SSD1306(0, 1, "Ins : ");
    int8_t insStrBuff[16];
    for(uint8_t i = 0 ; i < 16 ; i++) insStrBuff[i] = pgm_read_byte(&instrumentList[eeprom_read_byte(&selectedInstrument)].name[i]);
    printStr_SSD1306(6, 1, insStrBuff);

    menuButtonPrevious = menuButton;
    backButtonPrevious = backButton;
    menuIndex = 0;

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

        printStr_SSD1306(7, 4, frequencyString);
        printStr_SSD1306(7, 5, noteString);
    }else{
        printStr_SSD1306(7, 4, "    ");
        printStr_SSD1306(7, 5, "   ");
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
    printStr_SSD1306(6, 0, "Main Menu");
    
    menuButtonPrevious = menuButton;
    backButtonPrevious = backButton;
    menuIndex = 0;
    
    sei();
}

void mainMenuController(){
    for(uint8_t i = 0 ; i < N_MAINMENUITEMS ; i++){
        int8_t strBuff[18];
        for(uint8_t j = 0 ; j < 18 ; j++) strBuff[j] = pgm_read_byte(&mainMenuItems[i][j]);
        if(i == menuIndex) printStrInverted_SSD1306(0, i + 1, strBuff);
        else               printStr_SSD1306(0, i+1, strBuff);
    }
    

    if(incrementsModulator < -2){
        menuIndex++;
        if(menuIndex >= N_MAINMENUITEMS) menuIndex = 0;
        incrementsModulator = 0;
    }

    if(incrementsModulator > 2){
        menuIndex--;
        if(menuIndex >= N_MAINMENUITEMS) menuIndex = N_MAINMENUITEMS - 1;
        incrementsModulator = 0;
    }

    if(backButton != backButtonPrevious){
        if(backButton) mainScreenInit();
        backButtonPrevious = backButton;
    }

    if(menuButton != menuButtonPrevious){
        if(menuButton){
            switch(menuIndex){
                case 0:
                    instrumentMenuInit();
                    break;
                case 1:
                    break;
                case 2:
                    break;
            }
        }
        menuButtonPrevious = menuButton;
    }
}


void instrumentMenuInit(){
    cli();

    screenControlFunction = instrumentMenuController;

    clear_SSD1306();
    printStr_SSD1306(1, 0, "Instrument Select");
    
    menuButtonPrevious = menuButton;
    backButtonPrevious = backButton;
    menuIndex = 0;

    sei();
}

void instrumentMenuController(){
    for(uint8_t i = 0 ; i < N_INSTRUMENTS ; i++){
        int8_t strBuff[16];
        for(uint8_t j = 0 ; j < 16 ; j++) strBuff[j] = pgm_read_byte(&instrumentList[i].name[j]);
        if(i == menuIndex){
            printCharInverted_SSD1306(0, i+1, ' ');
            printCharInverted_SSD1306(1, i+1, pgm_read_byte(&instrumentList[i].icon));
            printCharInverted_SSD1306(2, i+1, ' ');
            printStrInverted_SSD1306 (3, i+1, strBuff);
        }else{
            printChar_SSD1306(0, i+1, ' ');
            printChar_SSD1306(1, i+1, pgm_read_byte(&instrumentList[i].icon));
            printChar_SSD1306(2, i+1, ' ');
            printStr_SSD1306 (3, i+1, strBuff);
        }
    }
    

    if(incrementsModulator < -2){
        menuIndex++;
        if(menuIndex >= N_INSTRUMENTS) menuIndex = 0;
        incrementsModulator = 0;
    }

    if(incrementsModulator > 2){
        menuIndex--;
        if(menuIndex >= N_INSTRUMENTS) menuIndex = N_MAINMENUITEMS - 1;
        incrementsModulator = 0;
    }

    if(backButton != backButtonPrevious){
        if(backButton) mainMenuInit();
        backButtonPrevious = backButton;
    }

    if(menuButton != menuButtonPrevious){
        if(menuButton){
            loadInstrument(menuIndex);
            eeprom_write_byte(&selectedInstrument, menuIndex);
            mainScreenInit();
        }
        menuButtonPrevious = menuButton;
    }
}