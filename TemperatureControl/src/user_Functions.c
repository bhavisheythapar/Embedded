#include "UART_Functions.h"
#include "user_Functions.h"

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
    char t_m[20], t_d[20];

    // Convert temperature values to strings
    sprintf(t_m,"%.2f",t_measured);
    sprintf(t_d,"%.2f",t_desired);

    transmitString(t_m);
    transmitString(",");
    transmitString(t_d);
    printCR();
}

