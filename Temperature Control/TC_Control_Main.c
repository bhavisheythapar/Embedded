#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#pragma clang diagnostic push
#pragma ide diagnostic ignored "EndlessLoop"
#define F_CPU 16000000UL
#define BAUD 9600
#define BUTTON 7

void initPowerControl(void);
void initUSART();
void initADC(void);
void PWM(uint8_t);
uint16_t analog (uint8_t);
float voltageToTemp (uint16_t);
void print (double, double);
void transmitString(char*);
void printCR();
void delay_ms (uint16_t);
void transmitByte (unsigned char);

static int counter=0;
static double T_init=0;
static char t_i[20];

int main(void)
{
    initPowerControl();
    initUSART();
    initADC();

    uint8_t channel = 0;

    // Default target temperature is 35
    double T_desired=35, T_measured=0;
    double error_current=0, error_previous=0, u=0, K_p=20, K_d=-30, de_dt=0;

    // Bulb default state is off
    PWM(0);

    int start_program_button=analog(BUTTON);

    while (start_program_button>100) {
        start_program_button=analog(BUTTON);
    }

    while (1)
    {
        int button=analog(BUTTON);

        // Increase temperature using S1 and decrease using S2 on Romeo board
        if (button>100 && button<300) {
            T_desired++;
            delay_ms(50);
        }

        if (button > 300 && button < 500) {
            T_desired--;
            delay_ms(50);
        }

        // Calculate the measured temperature
        T_measured=voltageToTemp(analog(channel));

        // Print all the values
        print(T_measured,T_desired);

        error_previous=error_current;
        error_current=T_desired - T_measured;
        de_dt=error_current-error_previous;
        u=K_p*error_current+K_d*de_dt;

        if (u>255)
            u=255;
        else if (u<0)
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

void PWM(uint8_t duty_cycle) {
    OCR0A = duty_cycle;
}

uint16_t analog (uint8_t channel)
{
    ADMUX  &=  0xF0;
    ADMUX  |=  channel;
    ADCSRA |= (1<<ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

float voltageToTemp (uint16_t ADC_Value) {
    float T_0=298.15, steinhart=0;
    float resistance=1023.0/ADC_Value-1.0;
    int B_0=3950;

    resistance=10000.0*resistance;
    steinhart=resistance/10000.0;
    steinhart=log(steinhart);
    steinhart /=B_0;
    steinhart += 1.0/T_0;
    steinhart=1.0/steinhart;
    steinhart -= 273.15;

    return steinhart;
}

void print(double t_measured,double t_desired) {


    char t_m[20], t_d[20], pwm[20];

    // Convert temperature values to strings
    sprintf(t_m,"%.2f",t_measured);
    sprintf(t_d,"%.2f",t_desired);

    transmitString(t_m);
    transmitString(",");
    transmitString(t_d);
    printCR();
}

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

void delay_ms (uint16_t ms)
{
    for (int i = 0; i < ms; i++)
        _delay_ms(1);
}

void transmitByte (unsigned char data)
{
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = data;
}

#pragma clang diagnostic pop