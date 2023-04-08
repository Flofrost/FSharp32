#include <avr/io.h>

void main(){
    
    DDRD = 0xF0; // PD7 output
    PORTD = 0xF0; // PD7 ON
                  
    return;
}