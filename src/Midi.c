void uartSendINT8(const uint8_t x){
    while(!(UCSR0A & 0x20));
    UDR0 = x;
}

void uartSendSTR(const char* s){
    for(uint8_t i = 0 ; s[i] ; i++){
        while(!(UCSR0A & 0x20));
        UDR0 = s[i];
    }
}