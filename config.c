#include "config.h"
#include <xc.h>

uint8_t config_int_osc(void){
    ACTCON = 0x00; //HFINTOSC tuned by OSCTUNE and updates by software to OSCTUNE allowed.
    OSCCON1 = 0b01100000; 
    OSCTUNE = 0b00011111;
    OSCFRQ = 0b00001000;
    OSCEN = 0b01010100;
    //should select 64MHz
    
    return 1;
}

uint8_t config_PPS(void){
    //unlock PPS
    INTCON0bits.GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0b0;
    INTCON0bits.GIE = 1;
    
    //reassign peripherals
    RA2PPS = 0x09;
    
    INTCON0bits.GIE = 0;
    PPSLOCK = 0x55;
    PPSLOCK = 0xAA;
    PPSLOCKbits.PPSLOCKED = 0b1;
    INTCON0bits.GIE = 1;
    
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
    
    ADCON0bits.ADON = 1; //ADC enabled
    ADCON0bits.CONT = 0; //turn off continuous operation
    ADCON0bits.CS = 0; //clock derived from Fosc
    ADCON0bits.FM = 1; //ADRES right-justified
    ADCON0bits.GO = 0; //don't do an ADC
    ADCON2bits.MD = 0b000; //put ADC into legacy mode
    ADCON3bits.TMD = 0b111; //not using interrupts for ADC, but setting this anyway - will generate ADC interrupt regardless of thresholds.
    ADCLKbits.CS = 15; //ADC clk is Fosc/32 - required for Tad
    ADREFbits.NREF = 0b0; //NRef is Vss
    ADREFbits.PREF = 0b000; //Pref is Vdd
    ADPRE = 0x00; //Precharge time is 0.
    ADACQ = 0; //acquisition time = 0
    ADCAP = 0x00; //no additiional capacitance
    ADCPbits.CPON = 0; //charge pump OFF
    
    return 1;
}

uint8_t config_PWM_CCP1(void){
    
    T2CONbits.ON = 1; //turn TMR2 ON
    T2CONbits.CKPS = 0b000; //1:1 prescaler
    T2CONbits.OUTPS = 0b0000; // 1:1 postscaler
    T2HLTbits.PSYNC = 1; //TMR2 prescaler synchronised to Fosc/4
    T2HLTbits.CKPOL = 0; //TMR2 increments on rising edge
    T2HLTbits.CKSYNC = 1; //ON bit is synchronised to TMR2 clk input
    T2HLTbits.MODE = 0b00000; //normal timer mode
    T2CLKCONbits.CS = 0b0001; //Fosc/4 input clock source for TMR2
    CCP1CONbits.CCP1EN = 0b1; //enable CCP1
    CCP1CONbits.FMT = 0b1; //left-align
    CCP1CONbits.MODE = 0b1111; //put into PWM mode
    CCPTMRS0bits.C1TSEL = 0b01; //choose TMR2 to be the timer for CCP1 to do PWM
    TRISA2 = 1; //Port RA2 (pin that CCP3 outputs from) is disabled (set as input)
    T2PR = 0xFF;
    CCPR1H = 0b00;
    CCPR1L = 0x00; //these two lines set PWM to 0V.
    CCP1IP = 1; //high priority interrupt
    CCP1IF = 0; //clear CCP1 interrupt flag
    TMR2IF = 0; //clear timer2 interrupt flag
    TMR2IE = 1; //enable interrupt TMR2
    CCP1IE = 1; //enable interrupt for CCP1
    
    return 1;
}


uint8_t config_TMR0(void){
    T0CON0bits.EN = 1; //enable TMR0
    T0CON0bits.MD16 = 0; //TMR0 in 8-bit mode
    T0CON0bits.OUTPS = 0b000; //postscaler = 1:1
    T0CON1bits.CS = 0b010; //Fosc/4 clock
    T0CON1bits.ASYNC = 0b0; //sync'd to Fosc/4
    T0CON1bits.CKPS = 0b0000; //prescaler is 1:1
    TMR0IP = 1; //high priority interrupt
    TMR0IF = 0; //clear interrupt flag
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
    config_interrupts();
    
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

uint8_t config_interrupts(void){
    INTCON0bits.IPEN = 0; //disable interrupt priority
    
    return 1;
}