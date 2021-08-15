#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <math.h>
#include <stdint.h>

#define F_CPU 16000000UL
#define BAUD 9600
#define BUTTON 7

void initPowerControl(void);
void initUSART();
void initADC(void);
void PWM(uint8_t);
uint16_t analog (uint8_t);
float voltageToTemp (uint16_t);
void print (double, double, double);
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
    double T_desired=30, T_measured=0, error=0, u=0, K_p=30;

    PWM(0);

    while (1)
    {
        int button=analog(BUTTON);

        if (button<100)
            T_desired++;
        if (button>100 && button<300)
            T_desired--;

        T_measured=voltageToTemp(analog(channel));

        print(T_measured,T_desired,u);

        error=T_desired - T_measured;
        u=K_p*error;

        if (u>255)
            u=255;
        else if (u<0)
            u=0;

        PWM(u);
        delay_ms(100);

        if (counter==0)
            counter++;
    }
}

void initPowerControl(void) {
    DDRD |= (1 << PD6); // PD6 is now an output
    TCCR0A |= (1 << COM0A1); // Set none-inverting mode
    TCCR0A |= (1 << WGM01) | (1 << WGM00); // Set fast PWM Mode
    TCCR0B |= (1 << CS01); // Set prescaler to 8 and starts PWM
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

// ADC Functions
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

void print(double t_measured,double t_desired, double u) {
    transmitByte(12);

    char t_m[20], t_d[20], pwm[20];
    char text1[]="Initial Temperature: ", text2[]="Target Temperature: ", text3[]="Current Temperature: ";

    if (counter==0)
        T_init=t_measured;

    sprintf(t_m,"%.2f ",t_measured);
    sprintf(t_d,"%.2f ",t_desired);
    sprintf(pwm,"%.1f ",u);
    sprintf(t_i,"%.2f ",T_init);

    transmitString(text1);
    transmitString(t_i);
    transmitString("ºC");
    printCR();

    transmitString(text2);
    transmitString(t_d);
    transmitString("ºC");
    printCR();

    transmitString(text3);
    transmitString(t_m);
    transmitString("ºC   ");
    transmitString("PWM Cycle: ");
    transmitString(pwm);
    transmitString("%");
    printCR();
    printCR();

    if (counter==0)
        counter++;
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