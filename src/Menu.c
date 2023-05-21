#include "Menu.h"

uint8_t menuButtonPrevious = 0, backButtonPrevious = 0, menuIndex = 0;
uint8_t editeeX, editeeY, editeeATR, editeeBuffer;
uint8_t* editeePTR;

void (*screenControlFunction)() = mainScreenController;
void (*returnToController)();

#define sizeofList(item) (sizeof(item) / sizeof(item[0]))
#define abs(x) ((x) < 0) ? -(x) : (x)

const int8_t mainMenuItems[][18] PROGMEM = {
    "Instruments   ",
    "Keyboard Modes",
    "Envelope      ",
    "Profiles      "
};
const int8_t keyboardModeMenuItems[][7] PROGMEM = {
    "Normal",
    "Toggle",
    "Burst "
};
const int8_t envelopeMenuItems[][8] PROGMEM = {
    "Attack ",
    "Decay  ",
    "Sustain",
    "Release"
};
const int8_t ADSRMenuItems[3][8] PROGMEM = {
    "Target ",
    "Step   ",
    "Delay  "
};

void mainScreenInit(){
    cli();

    screenControlFunction = mainScreenController;

    clear_SSD1306();
    printStr_SSD1306(0, 0, "OCTAVE : ", 0);

    switch(octave){
        case 0:
            printStr_SSD1306(9, 0, "12", 0);
            break;
        case 1:
            printStr_SSD1306(9, 0, "34", 0);
            break;
        case 2:
            printStr_SSD1306(9, 0, "56", 0);
            break;
        case 3:
            printStr_SSD1306(9, 0, "78", 0);
            break;
    }
    
    printStr_SSD1306(0, 1, "Ins : ", 0);
    int8_t insStrBuff[16];
    for(uint8_t i = 0 ; i < 16 ; i++) insStrBuff[i] = pgm_read_byte(&instrumentList[eeprom_read_byte(&selectedInstrument)].name[i]);
    printStr_SSD1306(6, 1, insStrBuff, 0);

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
                printStr_SSD1306(9, 0, "34", 0);
                break;
            case 1:
                printStr_SSD1306(9, 0, "45", 0);
                break;
            case 2:
                printStr_SSD1306(9, 0, "56", 0);
                break;
            case 3:
                printStr_SSD1306(9, 0, "67", 0);
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

        printStr_SSD1306(7, 4, frequencyString, 0);
        printStr_SSD1306(7, 5, noteString, 0);
    }else{
        printStr_SSD1306(7, 4, "    ", 0);
        printStr_SSD1306(7, 5, "   ", 0);
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
    printStr_SSD1306(6, 0, "Main Menu", 0);
    
    menuButtonPrevious = menuButton;
    backButtonPrevious = backButton;
    menuIndex = 0;
    
    sei();
}

void mainMenuController(){
    for(uint8_t i = 0 ; i < sizeofList(mainMenuItems) ; i++){
        int8_t strBuff[18];
        for(uint8_t j = 0 ; j < 18 ; j++) strBuff[j] = pgm_read_byte(&mainMenuItems[i][j]);
        printStr_SSD1306(0, i + 1, strBuff, i == menuIndex);
    }
    

    if(incrementsModulator < -2){
        menuIndex++;
        if(menuIndex >= sizeofList(mainMenuItems)) menuIndex = 0;
        incrementsModulator = 0;
    }

    if(incrementsModulator > 2){
        menuIndex--;
        if(menuIndex >= sizeofList(mainMenuItems)) menuIndex = sizeofList(mainMenuItems) - 1;
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
                    keyboardModeMenuInit();
                    break;
                case 2:
                    envelopeMenuInit();
                    break;
                case 3:
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
    printStr_SSD1306(2, 0, "Instrument Select", 0);
    
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
            printChar_SSD1306(0, i+1, ' ', ATTR_INVERTED);
            printChar_SSD1306(1, i+1, pgm_read_byte(&instrumentList[i].icon), ATTR_INVERTED);
            printChar_SSD1306(2, i+1, ' ', ATTR_INVERTED);
            printStr_SSD1306 (3, i+1, strBuff, ATTR_INVERTED);
        }else{
            printChar_SSD1306(0, i+1, ' ', 0);
            printChar_SSD1306(1, i+1, pgm_read_byte(&instrumentList[i].icon), 0);
            printChar_SSD1306(2, i+1, ' ', 0);
            printStr_SSD1306 (3, i+1, strBuff, 0);
        }
    }
    

    if(incrementsModulator < -2){
        menuIndex++;
        if(menuIndex >= N_INSTRUMENTS) menuIndex = 0;
        incrementsModulator = 0;
    }

    if(incrementsModulator > 2){
        menuIndex--;
        if(menuIndex >= N_INSTRUMENTS) menuIndex = N_INSTRUMENTS - 1;
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


void keyboardModeMenuInit(){
    cli();

    screenControlFunction = keyboardModeMenuController;

    clear_SSD1306();
    printStr_SSD1306(4, 0, "Keyboard Mode", 0);
    
    menuButtonPrevious = menuButton;
    backButtonPrevious = backButton;
    menuIndex = 0;

    sei();
}

void keyboardModeMenuController(){
    for(uint8_t i = 0 ; i < sizeofList(keyboardModeMenuItems) ; i++){
        int8_t strBuff[7];
        for(uint8_t j = 0 ; j < 7 ; j++) strBuff[j] = pgm_read_byte(&keyboardModeMenuItems[i][j]);
            printStr_SSD1306(0, i+1, strBuff, i == menuIndex);
    }
    

    if(incrementsModulator < -2){
        menuIndex++;
        if(menuIndex >= sizeofList(keyboardModeMenuItems)) menuIndex = 0;
        incrementsModulator = 0;
    }

    if(incrementsModulator > 2){
        menuIndex--;
        if(menuIndex >= sizeofList(keyboardModeMenuItems)) menuIndex = sizeofList(mainMenuItems) - 1;
        incrementsModulator = 0;
    }

    if(backButton != backButtonPrevious){
        if(backButton) mainMenuInit();
        backButtonPrevious = backButton;
    }

    if(menuButton != menuButtonPrevious){
        if(menuButton){
            loadKeyboardMode(menuIndex)
            eeprom_write_byte(&selectedKeyboardMode, menuIndex);
            mainScreenInit();
        }
        menuButtonPrevious = menuButton;
    }
}


void envelopeMenuInit(){
    cli();

    screenControlFunction = envelopeMenuController;

    clear_SSD1306();
    printStr_SSD1306(1, 0, "Envelope Generator", 0);
    
    menuButtonPrevious = menuButton;
    backButtonPrevious = backButton;
    menuIndex = 0;

    sei();
}

void envelopeMenuController(){
    for(uint8_t i = 0 ; i < sizeofList(envelopeMenuItems) ; i++){
        int8_t strBuff[8];
        for(uint8_t j = 0 ; j < 8 ; j++) strBuff[j] = pgm_read_byte(&envelopeMenuItems[i][j]);
        printStr_SSD1306(0, i+1, strBuff, i == menuIndex);
    }
    

    if(incrementsModulator < -2){
        menuIndex++;
        if(menuIndex >= sizeofList(envelopeMenuItems)) menuIndex = 0;
        incrementsModulator = 0;
    }

    if(incrementsModulator > 2){
        menuIndex--;
        if(menuIndex >= sizeofList(envelopeMenuItems)) menuIndex = sizeofList(envelopeMenuItems) - 1;
        incrementsModulator = 0;
    }

    if(backButton != backButtonPrevious){
        if(backButton) mainMenuInit();
        backButtonPrevious = backButton;
    }

    if(menuButton != menuButtonPrevious){
        if(menuButton){
            switch(menuIndex){
                case 0:
                    attackMenuInit();
                    break;
                case 1:
                    decayMenuInit();
                    break;
                case 2:
                    sustainMenuInit();
                    break;
                case 3:
                    releaseMenuInit();
                    break;
            }
        }
        menuButtonPrevious = menuButton;
    }
}


void valueEditInit(uint8_t* valuePtr, uint8_t x, uint8_t y, void (*returnTo)(), uint8_t flags){
    cli();

    screenControlFunction = valueEditController;

    editeeX = x;
    editeeY = y;
    editeeATR = flags;
    editeePTR = valuePtr;
    editeeBuffer = *valuePtr;
    returnToController = returnTo;
    
    menuButtonPrevious = menuButton;
    backButtonPrevious = backButton;

    sei();
}

void valueEditController(){
    
    printUInt8_SSD1306(editeeX, editeeY, editeeBuffer, ' ', editeeATR);

    if(incrementsModulator < -2){
        editeeBuffer--;
        incrementsModulator = 0;
    }

    if(incrementsModulator > 2){
        editeeBuffer++;
        incrementsModulator = 0;
    }

    if(backButton != backButtonPrevious){
        if(backButton) returnToController();
        backButtonPrevious = backButton;
    }

    if(menuButton != menuButtonPrevious){
        if(menuButton){
            *editeePTR = editeeBuffer;
            returnToController();
        }
        menuButtonPrevious = menuButton;
    }
}


void attackMenuInit(){
    cli();

    screenControlFunction = attackMenuController;

    clear_SSD1306();
    printStr_SSD1306(3, 0, "Attack Settings", 0);
    
    menuButtonPrevious = menuButton;
    backButtonPrevious = backButton;
    menuIndex = 0;

    sei();
}

void attackMenuController(){
    for(uint8_t i = 0 ; i < 3 ; i++){
        int8_t strBuff[8];
        for(uint8_t j = 0 ; j < 8 ; j++) strBuff[j] = pgm_read_byte(&ADSRMenuItems[i][j]);
        if(i == menuIndex){
            printStr_SSD1306(0, i+1, strBuff, ATTR_INVERTED);
            if     (i == 0) printUInt8_SSD1306(7, i+1, loadedEnvelope.attackTarget, ' ', ATTR_INVERTED);
            else if(i == 1) printUInt8_SSD1306(7, i+1, loadedEnvelope.attackStep,   ' ', ATTR_INVERTED);
            else if(i == 2) printUInt8_SSD1306(7, i+1, loadedEnvelope.attackDelay,  ' ', ATTR_INVERTED);
        }else{
            printStr_SSD1306        (0, i+1, strBuff, 0);
            if     (i == 0) printUInt8_SSD1306(7, i+1, loadedEnvelope.attackTarget, ' ', 0);
            else if(i == 1) printUInt8_SSD1306(7, i+1, loadedEnvelope.attackStep,   ' ', 0);
            else if(i == 2) printUInt8_SSD1306(7, i+1, loadedEnvelope.attackDelay,  ' ', 0);
        }
    }
    

    if(incrementsModulator < -2){
        menuIndex++;
        if(menuIndex >= sizeofList(ADSRMenuItems)) menuIndex = 0;
        incrementsModulator = 0;
    }

    if(incrementsModulator > 2){
        menuIndex--;
        if(menuIndex >= sizeofList(ADSRMenuItems)) menuIndex = sizeofList(ADSRMenuItems) - 1;
        incrementsModulator = 0;
    }

    if(backButton != backButtonPrevious){
        if(backButton) envelopeMenuInit();
        backButtonPrevious = backButton;
    }

    if(menuButton != menuButtonPrevious){
        if(menuButton){
            if     (menuIndex == 0) valueEditInit(&loadedEnvelope.attackTarget, 7, 1, attackMenuInit, ATTR_UNDERLINE | ATTR_INVERTED);
            else if(menuIndex == 1) valueEditInit(&loadedEnvelope.attackStep,   7, 2, attackMenuInit, ATTR_UNDERLINE | ATTR_INVERTED);
            else if(menuIndex == 2) valueEditInit(&loadedEnvelope.attackDelay,  7, 3, attackMenuInit, ATTR_UNDERLINE | ATTR_INVERTED);
        }
        menuButtonPrevious = menuButton;
    }
}


void decayMenuInit(){
    cli();

    screenControlFunction = decayMenuController;

    clear_SSD1306();
    printStr_SSD1306(4, 0, "Decay Settings", 0);
    
    menuButtonPrevious = menuButton;
    backButtonPrevious = backButton;
    menuIndex = 0;

    sei();
}

void decayMenuController(){
    for(uint8_t i = 0 ; i < 3 ; i++){
        int8_t strBuff[8];
        for(uint8_t j = 0 ; j < 8 ; j++) strBuff[j] = pgm_read_byte(&ADSRMenuItems[i][j]);
        if(i == menuIndex){
            printStr_SSD1306(0, i+1, strBuff, ATTR_INVERTED);
            if     (i == 0) printUInt8_SSD1306(7, i+1, loadedEnvelope.decayTarget, ' ', ATTR_INVERTED);
            else if(i == 1) printUInt8_SSD1306(7, i+1, loadedEnvelope.decayStep,   ' ', ATTR_INVERTED);
            else if(i == 2) printUInt8_SSD1306(7, i+1, loadedEnvelope.decayDelay,  ' ', ATTR_INVERTED);
        }else{
            printStr_SSD1306        (0, i+1, strBuff, 0);
            if     (i == 0) printUInt8_SSD1306(7, i+1, loadedEnvelope.decayTarget, ' ', 0);
            else if(i == 1) printUInt8_SSD1306(7, i+1, loadedEnvelope.decayStep,   ' ', 0);
            else if(i == 2) printUInt8_SSD1306(7, i+1, loadedEnvelope.decayDelay,  ' ', 0);
        }
    }
    

    if(incrementsModulator < -2){
        menuIndex++;
        if(menuIndex >= sizeofList(ADSRMenuItems)) menuIndex = 0;
        incrementsModulator = 0;
    }

    if(incrementsModulator > 2){
        menuIndex--;
        if(menuIndex >= sizeofList(ADSRMenuItems)) menuIndex = sizeofList(ADSRMenuItems) - 1;
        incrementsModulator = 0;
    }

    if(backButton != backButtonPrevious){
        if(backButton) envelopeMenuInit();
        backButtonPrevious = backButton;
    }

    if(menuButton != menuButtonPrevious){
        if(menuButton){
            if     (menuIndex == 0) valueEditInit(&loadedEnvelope.decayTarget, 7, 1, decayMenuInit, ATTR_UNDERLINE | ATTR_INVERTED);
            else if(menuIndex == 1) valueEditInit(&loadedEnvelope.decayStep,   7, 2, decayMenuInit, ATTR_UNDERLINE | ATTR_INVERTED);
            else if(menuIndex == 2) valueEditInit(&loadedEnvelope.decayDelay,  7, 3, decayMenuInit, ATTR_UNDERLINE | ATTR_INVERTED);
        }
        menuButtonPrevious = menuButton;
    }
}


void sustainMenuInit(){
    cli();

    screenControlFunction = sustainMenuController;

    clear_SSD1306();
    printStr_SSD1306(2, 0, "Sustain Settings", 0);
    
    menuButtonPrevious = menuButton;
    backButtonPrevious = backButton;
    menuIndex = 0;

    sei();
}

void sustainMenuController(){
    for(uint8_t i = 0 ; i < 2 ; i++){
        int8_t strBuff[8];
        for(uint8_t j = 0 ; j < 8 ; j++) strBuff[j] = pgm_read_byte(&ADSRMenuItems[i+1][j]);
        if(i == menuIndex){
            printStr_SSD1306(0, i+1, strBuff, ATTR_INVERTED);
            if(i) printUInt8_SSD1306(7, i+1, loadedEnvelope.sustainDelay, ' ', ATTR_INVERTED);
            else  printUInt8_SSD1306(7, i+1, loadedEnvelope.sustainStep,  ' ', ATTR_INVERTED);
        }else{
            printStr_SSD1306        (0, i+1, strBuff, 0);
            if(i) printUInt8_SSD1306(7, i+1, loadedEnvelope.sustainDelay, ' ', 0);
            else  printUInt8_SSD1306(7, i+1, loadedEnvelope.sustainStep,  ' ', 0);
        }
    }
    

    if(abs(incrementsModulator) > 2){
        menuIndex = !menuIndex;
        incrementsModulator = 0;
    }

    if(backButton != backButtonPrevious){
        if(backButton) envelopeMenuInit();
        backButtonPrevious = backButton;
    }

    if(menuButton != menuButtonPrevious){
        if(menuButton){
            if(menuIndex) valueEditInit(&loadedEnvelope.sustainDelay, 7, 2, sustainMenuInit, ATTR_UNDERLINE | ATTR_INVERTED);
            else          valueEditInit(&loadedEnvelope.sustainStep,  7, 1, sustainMenuInit, ATTR_UNDERLINE | ATTR_INVERTED);
        }
        menuButtonPrevious = menuButton;
    }
}


void releaseMenuInit(){
    cli();

    screenControlFunction = releaseMenuController;

    clear_SSD1306();
    printStr_SSD1306(2, 0, "Release Settings", 0);
    
    menuButtonPrevious = menuButton;
    backButtonPrevious = backButton;
    menuIndex = 0;

    sei();
}

void releaseMenuController(){
    for(uint8_t i = 0 ; i < 2 ; i++){
        int8_t strBuff[8];
        for(uint8_t j = 0 ; j < 8 ; j++) strBuff[j] = pgm_read_byte(&ADSRMenuItems[i+1][j]);
        if(i == menuIndex){
            printStr_SSD1306(0, i+1, strBuff, ATTR_INVERTED);
            if(i) printUInt8_SSD1306(7, i+1, loadedEnvelope.releaseDelay, ' ', ATTR_INVERTED);
            else  printUInt8_SSD1306(7, i+1, loadedEnvelope.releaseStep,  ' ', ATTR_INVERTED);
        }else{
            printStr_SSD1306        (0, i+1, strBuff, 0);
            if(i) printUInt8_SSD1306(7, i+1, loadedEnvelope.releaseDelay, ' ', 0);
            else  printUInt8_SSD1306(7, i+1, loadedEnvelope.releaseStep,  ' ', 0);
        }
    }
    

    if(abs(incrementsModulator) > 2){
        menuIndex = !menuIndex;
        incrementsModulator = 0;
    }

    if(backButton != backButtonPrevious){
        if(backButton) envelopeMenuInit();
        backButtonPrevious = backButton;
    }

    if(menuButton != menuButtonPrevious){
        if(menuButton){
            if(menuIndex) valueEditInit(&loadedEnvelope.releaseDelay, 7, 2, releaseMenuInit, ATTR_UNDERLINE | ATTR_INVERTED);
            else          valueEditInit(&loadedEnvelope.releaseStep,  7, 1, releaseMenuInit, ATTR_UNDERLINE | ATTR_INVERTED);
        }
        menuButtonPrevious = menuButton;
    }
}
