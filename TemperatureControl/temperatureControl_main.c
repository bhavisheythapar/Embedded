#include "user_Functions.h"
#include "AVR_Functions.h"
#include "UART_Functions.h"
#define F_CPU 16000000UL
#define BUTTON 7

int main(void)
{
    initPowerControl();
    initUSART();
    initADC();

    uint8_t channel = 0;

    // Default target temperature is 35
    double T_desired=40, T_measured;
    double error_current=0, error_previous, u, K_p=20, K_d=-30, de_dt;
    int button;

    // Bulb default state is off
    PWM(0);

    int start_program_button=analog(BUTTON);

    while (start_program_button>100) {
        start_program_button=analog(BUTTON);
    }

    while (1)
    {
        button=analog(BUTTON);

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
        T_measured = voltageToTemp(analog(channel));

        // Print all the values
        print(T_measured, T_desired);

        error_previous = error_current;
        error_current = T_desired - T_measured;
        de_dt = error_current - error_previous;
        u = K_p * error_current + K_d * de_dt;

        if (u > 255)
            u = 255;
        else if (u < 0)
            u = 0;

        PWM(u);

        delay_ms(100);
    }
}