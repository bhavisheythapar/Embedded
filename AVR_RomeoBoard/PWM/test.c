#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define SERIESRESISTOR 10000

void initPowerControl(void);
void PWM(uint8_t);
void initADC(void);
void initUSART();
void transmitByte (unsigned char);
void print (float, float);
uint16_t analog (uint8_t);
float voltageToTemp (uint16_t);
void delay_ms (uint16_t);
void transmitString(char*);
void printCR();
void printR();


        int main(void)
{
    initPowerControl();
    initUSART();
    initADC();
    uint8_t channel = 0;
    float T_desired=42;
    float T_measured=0;
    float e=0;
    float u=0;
    float K_p=200;
    PWM(0);

    while (1)
    {
        T_measured=voltageToTemp(analog(channel));
        print(T_measured,T_desired);
        e=T_desired - T_measured;
        u=K_p*e;
        if (u>255)
            u=255;
        if (u<0)
            u=0;
        PWM(u);
        delay_ms(100);
    }
}

void initPowerControl(void) {
    DDRD |= (1 << PD6); // PD6 is now an output
    TCCR0A |= (1 << COM0A1); // Set none-inverting mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00); // Set fast PWM Mode
    TCCR0B |= (1 << CS01); // Set prescaler to 8 and starts PWM
}

void PWM(uint8_t duty_cycle) {
    OCR0A = duty_cycle;
}

// ADC Functions
void initADC ()
{
    ADCSRA |= (1<<ADEN);
    ADMUX |= (1<<REFS0);
    ADCSRA |= (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
    ADCSRA |= (1<<ADSC);
}

uint16_t analog (uint8_t channel)
{
    ADMUX  &=  0xF0;
    ADMUX  |=  channel;
    ADCSRA |= (1<<ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

// USART Functions
void initUSART ()
{
    unsigned int ubrr = F_CPU/16/BAUD-1;
    UBRR0H = (unsigned char) (ubrr>>8);
    UBRR0L = (unsigned char) ubrr;
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);
}

void transmitByte (unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

float voltageToTemp (uint16_t ADC_Value) {
    float T_0=298.15;
    int B_0=3950;
    float resistance=1023.0/ADC_Value-1.0;
    resistance=10000.0*resistance;
    float steinhart;
    steinhart=resistance/10000.0;
    steinhart=log(steinhart);
    steinhart /=B_0;
    steinhart += 1.0/T_0;
    steinhart=1.0/steinhart;
    steinhart -= 273.15;
    return steinhart;
}

void print(float t_measured,float t_desired) {
    char t_m[20];
    char t_d[20];
    char text1[]="Target Temperature: ";
    char text2[]="Current Temperature: ";
    sprintf(t_m,"%f ",t_measured);
    sprintf(t_d,"%f ",t_desired);
    transmitString(text1);
    transmitString(t_d);
    printCR();
    transmitString(text2);
    transmitString(t_m);
    printCR();
    printCR();
}

void printCR() {
    char stringCR[3];
    sprintf(stringCR,"\r\n");
    transmitString(stringCR);
}

void printR() {
    char stringCR[3];
    sprintf(stringCR,"\r");
    transmitString(stringCR);
}
void transmitString(char* StringPtr) {
    while(*StringPtr != 0x00) {
        transmitByte(*StringPtr);
        StringPtr++;
    }
}



// Delay Function
void delay_ms (uint16_t ms)
{
    for (int i = 0; i < ms; i++)
        _delay_ms(1);
}