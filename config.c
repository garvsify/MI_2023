#include "config.h"
#include <xc.h>

uint8_t config_int_osc(void){
    ACTCON = 0x00; //HFINTOSC tuned by OSCTUNE and updates by software to OSCTUNE allowed.
    OSCTUNE = 0b00011111;
    OSCFRQ = 0b00001000;
    OSCEN = 0b01010100;
    //should select 64MHz
    
    return 1;
}

uint8_t config_PPS(void){
    RA2PPS = 0x09;
    
    return 1;
}

uint8_t turn_off_peripherals_not_required(void){
    PMD0 = 0b00011001; //Disable CRC, NVM Memory Scanner, and Interrupt-on-Change
    PMD1 = 0b11100000; //Disable Comparator 1, ZCD, and SMT1
    PMD2 = 0b01111101; //Disable, CWG1MD, DSM, NCO, Active Clock Tuning, DAC, and Comparator 1
    PMD3 = 0b11111111; //Disable UARTx, SPIx, I2C, PWMx
    PMD4 = 0b11111111; //Disable DMA, CRC, UARTx
    PMD5 = 0b00000011; //Disable DAC and DMAx 
    
    return 1;
}

uint8_t turn_on_watchdog(void){
    SWDTEN = 1;
    
    return 1;
}


uint8_t config_ports(void){
    ANSELC = 0b00001111; //set RC0-3 to analog inputs, RC5 as output
    TRISC = 0b00001111; //set RC0-3 to inputs, RC5 as output
    WPUC = 0b00000000; //disable pullups on port C
    
    return 1;
}


uint8_t config_ADC(void){
    ADCON0 = 0b10011010;
    ADPRE = 0;
    ADCON1 = 0x00;
    ADCON2 = 0x00;
    ADCON3 = 0x00;
    ADREF = 0x00;
    ADACQ = 0b0000000001010;
    ADCAP = 0;
    ADCP = 0;
    
    return 1;
}


uint8_t config_PWM_CCP1(void){
    //following steps in datasheet
    CCPTMRS0 = 0x01; //choose TMR2 to be the timer for CCP1 to do PWM
    TRISA2 = 1; //Port RA2 (pin that CCP3 outputs from) is disabled (set as input)
    T2PR = 0xFF; //set PR2 value to 255 (set the PWM frequency to 31.25kHz)
    CCP1CON = CCP1CON | 0b10011111; //put CCP1 into PWM mode, and left-align 10-bit TMR2 register H and L, and enable the peripheral.
    CCP1IE = 1; //enable interrupt for CCP1
    CCPR1H = 0b00;
    CCPR1L = 0x00; //these two lines set PWM to 0V.
    TMR2IF = 0; //clear timer2 interrupt flag
    T2HLT = 0b10100000;
    T2CLKCON = 0b00000001;
    T2CON = T2CON | 0b00000000; //Change Prescaler to 1:1, Postscaler to 1:1, and turn TMR2 OFF.
    TMR2IE = 1;
    
    return 1;
}


uint8_t config_TMR0(void){
    T0CON0 = 0b10000000;
    T0CON1 = 0b01000000; //set clock source of TMR0 to Fosc/4 and sync to Fosc
    TMR0IF = 0;
    TMR0IE = 1; //enable TMR0 interrupts
    
    return 1;
}


uint8_t config_system(void){
    config_int_osc();
    config_PPS();
    turn_off_peripherals_not_required();
    turn_on_watchdog();
    config_ports();
    config_ADC();
    config_PWM_CCP1();
    config_TMR0();
    config_global_interrupts();
    
    return 1;
}


uint8_t turn_on_CCP1_PWM(void){
    //this procedure ensures near as dammit that a the PWM output will start with a full cycle
    TMR2IF = 0; //make sure TMR2IF is cleared
    T2CON = T2CON | (0b1 << 7); //turn on timer2
    while(TMR2IF == 0){} //wait for TMR2 to overflow
    TMR2IF = 0; //clear TMR2IF
    TRISA2 = 0; //set RA2 to output CCP3 PWM
    
    return 1;
}

uint8_t config_global_interrupts(void){
    INTCON0 = 0; //disable interrupt priority
 
    return 1;
}