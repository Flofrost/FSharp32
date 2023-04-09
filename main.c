#include <avr/io.h>

void main(){
    
    DDRA = 0x02; // PD7 output
    PORTA = 0x02; // PD7 ON
                  
    return;
}