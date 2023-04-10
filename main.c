#include <avr/io.h>

void main(){
    
    DDRA = 0xFF;
    
    while(1) PORTA++;
                  
    return;
}