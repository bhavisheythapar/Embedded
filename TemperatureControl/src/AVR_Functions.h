#include <avr/io.h>
#include <util/delay.h>

#ifndef LAB1_AVR_INIT_FUNCTIONS_H
#define LAB1_AVR_INIT_FUNCTIONS_H

void initPowerControl(void);
void initUSART(void);
void initADC(void);
void delay_ms (uint16_t);

#endif
