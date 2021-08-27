#include <avr/io.h>
#include <math.h>


#ifndef LAB1_UART_FUNCTIONS_H
#define LAB1_UART_FUNCTIONS_H

void transmitString(char*);
void printCR(void);
void transmitByte (unsigned char);

#endif //LAB1_UART_FUNCTIONS_H
