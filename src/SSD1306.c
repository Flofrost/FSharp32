#include "SSD1306.h"
#include <avr/pgmspace.h>

// <------  Down
const uint8_t charmap[95][5] PROGMEM = {
    {0x00, 0x00, 0x00, 0x00, 0x00},            // Code for char Space
    {0x00, 0x00, 0x5F, 0x00, 0x00},            // Code for char !
    {0x00, 0x06, 0x00, 0x06, 0x00},            // Code for char "
    {0x14, 0x7F, 0x14, 0x7F, 0x14},            // Code for char #
    {0x04, 0x2A, 0x7F, 0x2A, 0x10},            // Code for char $
    {0x46, 0x30, 0x08, 0x06, 0x31},            // Code for char %
    {0x36, 0x49, 0x55, 0x22, 0x50},            // Code for char &
    {0x00, 0x00, 0x06, 0x00, 0x00},            // Code for char '
    {0x00, 0x3E, 0x41, 0x41, 0x00},            // Code for char (
    {0x00, 0x41, 0x41, 0x3E, 0x00},            // Code for char )
    {0x00, 0x14, 0x0E, 0x14, 0x00},            // Code for char *
    {0x08, 0x08, 0x3E, 0x08, 0x08},            // Code for char +
    {0x00, 0x40, 0x30, 0x00, 0x00},            // Code for char ,
    {0x08, 0x08, 0x08, 0x08, 0x08},            // Code for char -
    {0x00, 0x00, 0x20, 0x00, 0x00},            // Code for char .
    {0x40, 0x30, 0x08, 0x06, 0x01},            // Code for char /
    {0x3E, 0x51, 0x49, 0x45, 0x3E},            // Code for char 0
    {0x44, 0x42, 0x7F, 0x40, 0x40},            // Code for char 1
    {0x42, 0x61, 0x51, 0x49, 0x46},            // Code for char 2
    {0x22, 0x41, 0x49, 0x49, 0x36},            // Code for char 3
    {0x18, 0x14, 0x12, 0x7F, 0x10},            // Code for char 4
    {0x27, 0x49, 0x49, 0x49, 0x31},            // Code for char 5
    {0x3E, 0x49, 0x49, 0x49, 0x32},            // Code for char 6
    {0x41, 0x21, 0x19, 0x05, 0x03},            // Code for char 7
    {0x36, 0x49, 0x49, 0x49, 0x36},            // Code for char 8
    {0x26, 0x49, 0x49, 0x49, 0x3E},            // Code for char 9
    {0x00, 0x00, 0x36, 0x00, 0x00},            // Code for char :
    {0x00, 0x40, 0x36, 0x00, 0x00},            // Code for char ;
    {0x08, 0x14, 0x22, 0x41, 0x00},            // Code for char <
    {0x14, 0x14, 0x14, 0x14, 0x14},            // Code for char =
    {0x00, 0x41, 0x22, 0x14, 0x08},            // Code for char >
    {0x02, 0x01, 0x51, 0x09, 0x06},            // Code for char ?
    {0x3E, 0x41, 0x4D, 0x49, 0x2E},            // Code for char @
    {0x7E, 0x09, 0x09, 0x09, 0x7E},            // Code for char A
    {0x7F, 0x49, 0x49, 0x49, 0x36},            // Code for char B
    {0x3E, 0x41, 0x41, 0x41, 0x22},            // Code for char C
    {0x7F, 0x41, 0x41, 0x41, 0x3E},            // Code for char D
    {0x7F, 0x49, 0x49, 0x49, 0x41},            // Code for char E
    {0x7F, 0x09, 0x09, 0x01, 0x01},            // Code for char F
    {0x3E, 0x41, 0x49, 0x49, 0x3A},            // Code for char G
    {0x7F, 0x08, 0x08, 0x08, 0x7F},            // Code for char H
    {0x41, 0x41, 0x7F, 0x41, 0x41},            // Code for char I
    {0x21, 0x41, 0x41, 0x3F, 0x01},            // Code for char J
    {0x7F, 0x08, 0x14, 0x22, 0x41},            // Code for char K
    {0x7F, 0x40, 0x40, 0x40, 0x40},            // Code for char L
    {0x7F, 0x02, 0x0C, 0x02, 0x7F},            // Code for char M
    {0x7F, 0x02, 0x1C, 0x20, 0x7F},            // Code for char N
    {0x3E, 0x41, 0x41, 0x41, 0x3E},            // Code for char O
    {0x7F, 0x09, 0x09, 0x09, 0x06},            // Code for char P
    {0x3E, 0x41, 0x51, 0x21, 0x5E},            // Code for char Q
    {0x7F, 0x09, 0x19, 0x29, 0x46},            // Code for char R
    {0x26, 0x49, 0x49, 0x49, 0x32},            // Code for char S
    {0x01, 0x01, 0x7F, 0x01, 0x01},            // Code for char T
    {0x3F, 0x40, 0x40, 0x40, 0x3F},            // Code for char U
    {0x0F, 0x30, 0x40, 0x30, 0x0F},            // Code for char V
    {0x7F, 0x20, 0x18, 0x20, 0x7F},            // Code for char W
    {0x41, 0x36, 0x08, 0x36, 0x41},            // Code for char X
    {0x01, 0x06, 0x78, 0x06, 0x01},            // Code for char Y
    {0x61, 0x51, 0x49, 0x45, 0x43},            // Code for char Z
    {0x00, 0x7F, 0x41, 0x41, 0x00},            // Code for char [
    {0x01, 0x06, 0x08, 0x30, 0x40},            // Code for char BackSlash
    {0x00, 0x41, 0x41, 0x7F, 0x00},            // Code for char ]
    {0x08, 0x04, 0x02, 0x04, 0x08},            // Code for char ^
    {0x40, 0x40, 0x40, 0x40, 0x40},            // Code for char _
    {0x00, 0x01, 0x02, 0x04, 0x00},            // Code for char `
    {0x20, 0x54, 0x54, 0x54, 0x78},            // Code for char a
    {0x7C, 0x50, 0x50, 0x50, 0x20},            // Code for char b
    {0x38, 0x44, 0x44, 0x44, 0x28},            // Code for char c
    {0x20, 0x50, 0x50, 0x50, 0x7C},            // Code for char d
    {0x38, 0x54, 0x54, 0x54, 0x58},            // Code for char e
    {0x00, 0x78, 0x14, 0x04, 0x00},            // Code for char f
    {0x08, 0x54, 0x54, 0x54, 0x3C},            // Code for char g
    {0x7C, 0x10, 0x10, 0x10, 0x60},            // Code for char h
    {0x00, 0x00, 0x74, 0x00, 0x00},            // Code for char i
    {0x00, 0x40, 0x34, 0x00, 0x00},            // Code for char j
    {0x7C, 0x10, 0x28, 0x44, 0x00},            // Code for char k
    {0x00, 0x00, 0x3C, 0x40, 0x00},            // Code for char l
    {0x7C, 0x04, 0x18, 0x04, 0x78},            // Code for char m
    {0x7C, 0x04, 0x04, 0x04, 0x78},            // Code for char n
    {0x38, 0x44, 0x44, 0x44, 0x38},            // Code for char o
    {0x7C, 0x14, 0x14, 0x14, 0x08},            // Code for char p
    {0x08, 0x14, 0x14, 0x14, 0x7C},            // Code for char q
    {0x7C, 0x08, 0x04, 0x04, 0x04},            // Code for char r
    {0x48, 0x54, 0x54, 0x54, 0x24},            // Code for char s
    {0x00, 0x3C, 0x48, 0x40, 0x00},            // Code for char t
    {0x3C, 0x40, 0x40, 0x40, 0x7C},            // Code for char u
    {0x0C, 0x30, 0x40, 0x30, 0x0C},            // Code for char v
    {0x7C, 0x40, 0x30, 0x40, 0x3C},            // Code for char w
    {0x44, 0x28, 0x10, 0x28, 0x44},            // Code for char x
    {0x04, 0x48, 0x30, 0x08, 0x04},            // Code for char y
    {0x44, 0x64, 0x54, 0x4C, 0x44},            // Code for char z
    {0x08, 0x36, 0x41, 0x41, 0x00},            // Code for char {
    {0x00, 0x00, 0x7F, 0x00, 0x00},            // Code for char |
    {0x00, 0x41, 0x41, 0x36, 0x08},            // Code for char }
    {0x08, 0x04, 0x08, 0x10, 0x08},            // Code for char ~
};

const int8_t hexChars[16] PROGMEM = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};


void init_SSD1306(){
    COMMAND_SSD1306(0xAF); // Turn ON
    COMMAND_SSD1306(0xA4); // Display RAM
    COMMAND_SSD1306(0xA6); // Non inverted
    COMMAND_SSD1306(0xA1); // Flip Screen
    COMMAND_SSD1306(0xC8); // Mirror Screen
    COMMAND_SSD1306(0x20);
    COMMAND_SSD1306(0x02); // Page addressing mode
    COMMAND_SSD1306(0x8D);
    COMMAND_SSD1306(0x14); // Enable charge pump
}

void clear_SSD1306(){
    for(uint8_t page = 0 ; page < 8 ; page++){
        COMMAND_SSD1306(0x00);
        COMMAND_SSD1306(0x10);
        COMMAND_SSD1306(0xB0 | page);
        
        I2C_START();
        I2C_WAIT_TRASMISSION();
        I2C_WRITE(ADDR_W_SSD1306);
        I2C_WAIT_TRASMISSION();
        I2C_WRITE(0x40);
        I2C_WAIT_TRASMISSION();
        for(uint16_t i = 0; i < 128; i++){
            I2C_WRITE(0x00);
            I2C_WAIT_TRASMISSION();
        }
        I2C_STOP();
    }
}

void printChar_SSD1306(uint8_t x, uint8_t y, int8_t c){
    c -= 0x20;
    x *= 6;

    COMMAND_SSD1306(0x00 | (x & 0x0F));
    COMMAND_SSD1306(0x10 | (x >> 4));
    COMMAND_SSD1306(0xB0 | (y & 0x07));

    I2C_START();
    I2C_WAIT_TRASMISSION();
    I2C_WRITE(ADDR_W_SSD1306);
    I2C_WAIT_TRASMISSION();
    I2C_WRITE(0x40);
    I2C_WAIT_TRASMISSION();
    for(uint8_t i = 0; i < 5; i++){
        I2C_WRITE(pgm_read_byte(&charmap[c][i]));
        I2C_WAIT_TRASMISSION();
    }
    I2C_WRITE(0x00);
    I2C_WAIT_TRASMISSION();
    I2C_STOP();
}

void printStr_SSD1306(uint8_t x, uint8_t y, char* s){
    x *= 6;
    
    COMMAND_SSD1306(0x00 | (x & 0x0F));
    COMMAND_SSD1306(0x10 | (x >> 4));
    COMMAND_SSD1306(0xB0 | (y & 0x07));
    
    I2C_START();
    I2C_WAIT_TRASMISSION();
    I2C_WRITE(ADDR_W_SSD1306);
    I2C_WAIT_TRASMISSION();
    I2C_WRITE(0x40);
    I2C_WAIT_TRASMISSION();
    for(uint8_t i = 0 ; s[i] ; i++){
        const int8_t character = s[i] - 0x20;
        for(uint8_t col = 0 ; col < 5 ; col++){
            I2C_WRITE(pgm_read_byte(&charmap[character][col]));
            I2C_WAIT_TRASMISSION();
        }
        I2C_WRITE(0x00);
        I2C_WAIT_TRASMISSION();
    }
    I2C_STOP();
}

void printUInt8_SSD1306(uint8_t x, uint8_t y, uint8_t v, int8_t filler){
    int8_t stringBuffer[4];
    uint8_t numBuffer;
    uint8_t index = 0, ok = 0;

    if(numBuffer = (v / 100)){
        stringBuffer[index++] = numBuffer + 0x30;
        if(!ok) ok = 1;
    }else if(filler) stringBuffer[index++] = filler;

    if((numBuffer = ((v / 10) % 10)) || ok){
        stringBuffer[index++] = numBuffer + 0x30;
        if(!ok) ok = 1;
    }else if(filler) stringBuffer[index++] = filler;

    stringBuffer[index++] = (v % 10) + 0x30;

    stringBuffer[index] = 0;

    printStr_SSD1306(x, y, stringBuffer);
}

void printHex8_SSD1306(uint8_t x, uint8_t y, uint8_t v){
    int8_t stringBuffer[3];

    stringBuffer[0] = pgm_read_byte(&hexChars[(v >> 4) & 0x0F]);
    stringBuffer[1] = pgm_read_byte(&hexChars[v & 0x0F]);
    stringBuffer[2] = 0;

    printStr_SSD1306(x, y, stringBuffer);
}

void printHex16_SSD1306(uint8_t x, uint8_t y, uint16_t v){
    int8_t stringBuffer[5];

    stringBuffer[0] = pgm_read_byte(&hexChars[(v >> 12) & 0x0F]);
    stringBuffer[1] = pgm_read_byte(&hexChars[(v >> 8) & 0x0F]);
    stringBuffer[2] = pgm_read_byte(&hexChars[(v >> 4) & 0x0F]);
    stringBuffer[3] = pgm_read_byte(&hexChars[v & 0x0F]);
    stringBuffer[4] = 0;

    printStr_SSD1306(x, y, stringBuffer);
}

void printHex32_SSD1306(uint8_t x, uint8_t y, uint32_t v){
    int8_t stringBuffer[9];

    stringBuffer[0] = pgm_read_byte(&hexChars[(v >> 28) & 0x0F]);
    stringBuffer[1] = pgm_read_byte(&hexChars[(v >> 24) & 0x0F]);
    stringBuffer[2] = pgm_read_byte(&hexChars[(v >> 20) & 0x0F]);
    stringBuffer[3] = pgm_read_byte(&hexChars[(v >> 16) & 0x0F]);
    stringBuffer[4] = pgm_read_byte(&hexChars[(v >> 12) & 0x0F]);
    stringBuffer[5] = pgm_read_byte(&hexChars[(v >> 8) & 0x0F]);
    stringBuffer[6] = pgm_read_byte(&hexChars[(v >> 4) & 0x0F]);
    stringBuffer[7] = pgm_read_byte(&hexChars[v & 0x0F]);
    stringBuffer[8] = 0;

    printStr_SSD1306(x, y, stringBuffer);
}
