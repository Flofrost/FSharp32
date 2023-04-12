#include <avr/io.h>

char sin_lowu8(unsigned char x){
    unsigned char input = x & 0x7F;
    unsigned short res = input << 2;
    unsigned short buffer = input * input;
    buffer >>= 5;
    res -= buffer + 1;
    if(x & 0x80) return -res;
    else         return res;
}

void main(){
    
    DDRA = 0xFF;
    unsigned char counter = 0;
    
    while(1){
        PORTA = 127 + (sin_lowu8(counter) >> 2);
        counter += 2;
    }
                  
    return;
}