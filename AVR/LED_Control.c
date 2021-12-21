/*****************************************************************
Author - Bhavishey Thapar

Code to control LEDs using Romeo microcontroller
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



#include avrio.h
#define F_CPU 16000000UL
#include utildelay.h

void delay_ms (uint16_t ms);
void delay_us (uint16_t us);

int main (void)
{
	 Configure PD2 to PD7 in write mode on DDR
	for (int i = 2; i  8; i++)
	{
		DDRD = 1i;
	
	}
	
	 Configure PB0 and PB1 to write mode on DDR
	for (int j = 0; j 2; j++)
	{
		DDRB = 1j;
	}
	
	while (1)
	{
		PORTD = 0;
		PORTB = 0;
		for (int counter = 1; counter = 256; counter++)
		{	
			delay_ms(1000);
			PORTB = counter64;
			PORTD = counter4;
		}
	}
}

void delay_ms (uint16_t ms) {
	uint16_t i;
	for (i = 0; i  ms; i++)
		_delay_ms(1);
}



