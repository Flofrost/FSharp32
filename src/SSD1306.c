#include "SSD1306.h"
#include <avr/pgmspace.h>

// <------  Down
const unsigned char charmap[95][5] PROGMEM = {
    { // Space 0x20
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
        0b00000000,
    },
    { // ! 0x21
        0b00000000,
        0b00000000,
        0b10111100,
        0b00000000,
        0b00000000,
    },
    { // " 0x22
        0b00000000,
        0b00001100,
        0b00000000,
        0b00001100,
        0b00000000,
    },
    { // # 0x23
        0b00101000,
        0b11111110,
        0b00101000,
        0b11111110,
        0b00101000,
    }
};


void init_SSD1306(){
    COMMAND_SSD1306(0xAF); // Turn ON
    COMMAND_SSD1306(0xA4); // Display RAM
    COMMAND_SSD1306(0xA6); // Non inverted
    COMMAND_SSD1306(0xC8); // Flip Screen
    COMMAND_SSD1306(0x20);
    COMMAND_SSD1306(0x02); // Page addressing mode
    COMMAND_SSD1306(0x8D);
    COMMAND_SSD1306(0x14); // Enable charge pump
}

void clear_SSD1306(){
    for(unsigned char page = 0 ; page < 8 ; page++){
        COMMAND_SSD1306(0x00);
        COMMAND_SSD1306(0x10);
        COMMAND_SSD1306(0xB0 | page);
        
        I2C_START();
        I2C_WAIT_TRASMISSION();
        I2C_WRITE(ADDR_W_SSD1306);
        I2C_WAIT_TRASMISSION();
        I2C_WRITE(0x40);
        I2C_WAIT_TRASMISSION();
        for(unsigned short i = 0; i < 128; i++){
            I2C_WRITE(0x00);
            I2C_WAIT_TRASMISSION();
        }
        I2C_STOP();
    }
}

void printChar_SSD1306(unsigned char x, unsigned char y, char c){
    const char character = c - 0x20;

    COMMAND_SSD1306(0x00 | (x & 0x0F));
    COMMAND_SSD1306(0x10 | (x >> 4));
    COMMAND_SSD1306(0xB0 | (y & 0x07));

    I2C_START();
    I2C_WAIT_TRASMISSION();
    I2C_WRITE(ADDR_W_SSD1306);
    I2C_WAIT_TRASMISSION();
    I2C_WRITE(0x40);
    I2C_WAIT_TRASMISSION();
    for(unsigned char i = 0; i < 5; i++){
        I2C_WRITE(pgm_read_byte(&charmap[character][i]));
        I2C_WAIT_TRASMISSION();
    }
    I2C_WRITE(0x00);
    I2C_WAIT_TRASMISSION();
    I2C_STOP();
}
