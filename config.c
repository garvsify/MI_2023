#include "config.h"

uint8_t config_int_osc(void){
    ACTCON = 0x00; //HFINTOSC tuned by OSCTUNE and updates by software to OSCTUNE allowed.
    OSCTUNE = 0b00011111;
    OSCFRQ = 0b00001000;
    OSCEN = 0b01010100;
    //should select 64MHz
    return 1;
}

uint8_t config_PPS(void){
    
    return 1;
}

uint8_t turn_off_peripherals_not_required(){
    PMD0 = 0b00011001; //Disable CRC, NVM Memory Scanner, and Interrupt-on-Change
    PMD1 = 0b11100000; //Disable Comparator 1, ZCD, and SMT1
    PMD2 = 0b01111101; //Disable, CWG1MD, DSM, NCO, Active Clock Tuning, DAC, and Comparator 1
    PMD3 = 0b11111111; //Disable UARTx, SPIx, I2C, PWMx
    PMD4 = 0b11111111; //Disable DMA, CRC, UARTx
    PDM5 = 0b00000011; //Disable DAC and DMAx 
    return 1;
}


uint8_t config_ports(void){
    ANSELC = 0b00001111; //set RC0-3 to analog inputs, RC5 as output
    TRISC = 0b00001111; //set RC0-3 to inputs, RC5 as output
    WPUC = 0b00000000; //disable pullups on port C
    return 1;
}


uint8_t config_ADC_pins(void){
    ADCON1 = 0b10100000; //set ADC references to VDD and VSS, choose ADC clock source to be system_clock/32, and right-justify the ADC result registers.
    return 1;
}


uint8_t config_PWM_CCP3(void){
    SRLEN = 0; //disable SR latch from interfering with CCP3 output (on same pin))
    C1ON = 0; //disable comparator from interfering with CCP3 output (on same pin))
    //following steps in datasheet
    TRISA2 = 1; //Port RA2 (pin that CCP3 outputs from) is disabled (set as input)
    PR2 = 0xFF; //set PR2 value to 255 (set the PWM frequency to 31.25kHz)
    CCP3CON = CCP3CON | 0b00001111; //put CCP3 into PWM mode. 11xx puts CCP3 into PWM mode.
    //set_duty_CCP3(&duty); //pass the function the address of duty so the duty cycle can be set. For config, this will be set to 0.
    CCPTMRS0 = 0x00; //choose timer2 to be the timer for CCP3 to do PWM (also selects timer2 for the other PWM modules, should be okay?)
    TMR2IF = 0; //clear timer2 interrupt flag
    T2CON = T2CON | 0b00000000; //Change T2CKPS (bits 1 and 0) (timer2 prescaler) to 1x
    
    //the above steps should configure the PWM of CCP3 and set the PWM frequency to 31.25kHz

    return 1;
}


uint8_t config_TMR0_interrupt(void){
    TMR0IE = 1; //enable TMR0 interrupts
    return 1;
}


uint8_t config_system(void){
    config_int_osc();
    config_ports();
    config_ADC_pins();
    config_PWM_CCP3();
    return 1;
}


uint8_t turn_on_CCP3_PWM(void){
    //this procedure ensures near as dammit that a the PWM output will start with a full cycle
    TMR2IF = 0; //make sure TMR2IF is cleared
    TMR2ON = 1; //turn on timer2
    while(TMR2IF == 0){} //wait for TMR2 to overflow
    TMR2IF = 0; //clear TMR2IF
    TRISA2 = 0; //set RA2 to output CCP3 PWM
    return 1;
}