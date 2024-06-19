#ifndef CONFIG_H

    #define CONFIG_H

    #include <xc.h>
    #include <stdio.h>
    #include <math.h>

    // CONFIG1
    #pragma config FOSC = INTOSC    // Oscillator Selection (INTOSC oscillator: I/O function on CLKIN pin)
    #pragma config WDTE = OFF       // Watchdog Timer Enable (WDT disabled)
    #pragma config PWRTE = OFF      // Power-up Timer Enable (PWRT disabled)
    #pragma config MCLRE = ON       // MCLR Pin Function Select (MCLR/VPP pin function is MCLR)
    #pragma config CP = OFF         // Flash Program Memory Code Protection (Program memory code protection is disabled)
    #pragma config CPD = OFF        // Data Memory Code Protection (Data memory code protection is disabled)
    #pragma config BOREN = OFF      // Brown-out Reset Enable (Brown-out Reset disabled)
    #pragma config CLKOUTEN = OFF   // Clock Out Enable (CLKOUT function is disabled. I/O or oscillator function on the CLKOUT pin)
    #pragma config IESO = OFF       // Internal/External Switchover (Internal/External Switchover mode is disabled)
    #pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable (Fail-Safe Clock Monitor is disabled)

    // CONFIG2
    #pragma config WRT = OFF        // Flash Memory Self-Write Protection (Write protection off)
    #pragma config PLLEN = ON      // PLL Enable (4x PLL disabled)
    #pragma config STVREN = ON      // Stack Overflow/Underflow Reset Enable (Stack Overflow or Underflow will cause a Reset)
    #pragma config BORV = LO        // Brown-out Reset Voltage Selection (Brown-out Reset Voltage (Vbor), low trip point selected.)
    #pragma config DEBUG = ON       // In-Circuit Debugger Mode (In-Circuit Debugger enabled, ICSPCLK and ICSPDAT are dedicated to the debugger)
    #pragma config LVP = OFF         // Low-Voltage Programming Enable (Low-voltage programming disabled)

    // #pragma config statements should precede project file includes.

    #define _XTAL_FREQ 32000000

    uint8_t CONFIG_INT_OSC(void);
    uint8_t CONFIG_PORTS(void);
    uint8_t CONFIG_ADC_PINS(void);
    uint16_t DO_ADC(const uint8_t *modifier);
    uint8_t CONFIG_PWM_CCP3(void);
    uint8_t CONFIG_TMR0_INTERRUPT(void);
    uint8_t CONFIG_SYSTEM(void);
    uint8_t TURN_ON_CCP3_PWM(void);

#endif