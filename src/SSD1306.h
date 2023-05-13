#ifndef SSD1306_H
#define SSD1306_H

#include <avr/io.h>
#include "I2C.h"


#define ADDR_W_SSD1306 0x78

#define COMMAND_SSD1306(command) \
    I2C_START(); \
    I2C_WAIT_TRASMISSION(); \
    I2C_WRITE(ADDR_W_SSD1306); \
    I2C_WAIT_TRASMISSION(); \
    I2C_WRITE(0x00); \
    I2C_WAIT_TRASMISSION(); \
    I2C_WRITE(command); \
    I2C_WAIT_TRASMISSION(); \
    I2C_STOP()

void init_SSD1306();
void clear_SSD1306();
void printChar_SSD1306(uint8_t x, uint8_t y, int8_t c);
void printStr_SSD1306(uint8_t x, uint8_t y, char* s);
void printUInt8_SSD1306(uint8_t x, uint8_t y, uint8_t v, int8_t filler);
void printHex8_SSD1306(uint8_t x, uint8_t y, uint8_t v);
void printHex16_SSD1306(uint8_t x, uint8_t y, uint16_t v);
void printHex32_SSD1306(uint8_t x, uint8_t y, uint32_t v);

#endif