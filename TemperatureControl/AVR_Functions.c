#include "AVR_Functions.h"
#define BAUD 9600

void initPowerControl(void) {
    DDRD |= (1 << PD6); // PD6 is now an output
    TCCR0A |= (1 << COM0A1); // Set none-inverting mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00); // Set fast PWM Mode
    TCCR0B |= (1 << CS01); // Set prescaler to 8 and starts PWM
}

void initUSART ()
{
    unsigned int ubrr = F_CPU/16/BAUD-1;
    UBRR0H = (unsigned char) (ubrr>>8);
    UBRR0L = (unsigned char) ubrr;
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
}

void initADC ()
{
    ADCSRA |= (1<<ADEN);
    ADMUX |= (1<<REFS0);
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
    ADCSRA |= (1<<ADSC);
}

void delay_ms (uint16_t ms)
{
    for (int i = 0; i < ms; i++)
        _delay_ms(1);
}
