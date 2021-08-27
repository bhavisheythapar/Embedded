#include "UART_Functions.h"

void transmitString(char* StringPtr) {
    while(*StringPtr != 0x00) {
        transmitByte(*StringPtr);
        StringPtr++;
    }
}

void printCR() {
    char stringCR[3];
    sprintf(stringCR,"\r\n");
    transmitString(stringCR);
}


void transmitByte (unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}
