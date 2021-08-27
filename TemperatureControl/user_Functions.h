#include <stdio.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#ifndef LAB1_USER_FUNCTIONS_H
#define LAB1_USER_FUNCTIONS_H

void PWM(uint8_t);
uint16_t analog (uint8_t);
float voltageToTemp (uint16_t);
void print (double, double);

#endif