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
void printChar_SSD1306(unsigned char x, unsigned char y, char c);

#endif