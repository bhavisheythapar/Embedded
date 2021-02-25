/*****************************************************************
Author - Bhavishey Thapar
Program - Lab 2A to control LEDs using Romeo microcontroller
Date - 13th June, 2018
Version - 1.0
******************************************************************/

/* Connection Diagram

Atmega 328P		Romeo Board		IO Board Jumper		Component
PD2 ->			D2 ->			JP3_1 ->			D1A
PD3 ->			D3 ->			JP3_2 ->			D1B
PD4 ->			D4 ->			JP3_3 ->			D1C
PD5 ->			D5 ->			JP3_4 ->			D1D
PD6 ->			D6 ->			JP3_5 ->			D1E
PD7 ->			D7 ->			JP3_6 ->			D1F
PB0 ->			D8 ->			JP3_7 ->			D1G
PB1 ->			D9 ->			JP3_8 ->			D1H

*/

#include <avr/io.h>
#include <util/delay.h>

void delay_ms (uint16_t ms);

int main (void)
{
	// Configure PD2 to PD7 in write mode on DDR
	for (int i = 2; i < 8; i++)
	{
		DDRD |= 1<<i;
	}

	// Configure PB0 and PB1 to write mode on DDR
	for (int j = 0; j <2; j++)
	{
		DDRB |= 1<<j;
	}

	while (1)
	{
		PORTD = 0;
		PORTB = 0;
		for (int counter = 1; counter <= 256; counter++)
		{
		    if (counter>100)
                _delay_ms(1000);
		    else
		        _delay_ms(10);
			PORTB = counter/64;
			PORTD = counter*4;
		}
	}
}
