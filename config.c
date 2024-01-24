#include "config.h"

uint8_t CONFIG_INT_OSC(void){
    OSCCON = 0b11110000; //defer clock selection to configuration word 1, also select 32MHz as the system clock, and turn on PLL. 
    OSCTUNE = 0b00011111; //run oscillator as fast as possible
    //should select 32MHz
    return 1;
}


uint8_t CONFIG_PORTS(void){
    ANSELC = 0b00001111; //set RC0-3 to analog inputs, RC5 as output
    TRISC = 0b00001111; //set RC0-3 to inputs, RC5 as output
    WPUC = 0b00000000; //disable pullups on port C
    return 1;
}


uint8_t CONFIG_ADC_PINS(void){
    ADCON1 = 0b10100000; //set ADC references to VDD and VSS, choose ADC clock source to be system_clock/32, and right-justify the ADC result registers.
    return 1;
}


uint16_t DO_ADC(const uint8_t *modifier){
    ADCON0 = 0x00; //clear ADCON0 first
    //uint8_t temp = A2D_lookup[*modifier];
    uint8_t temp = (uint8_t)(*modifier << 2);
    ADCON0 = ADCON0 | temp; //select appropriate ADC channel
    ADON = 1; //turn on ADC
    __delay_ms(0.005); //acquisition time
    GO_nDONE = 1; //start A2D
    while(GO_nDONE == 1){} //do nothing while ADC conversion in progress
    ADON = 0; //turn off ADC
    uint16_t adc_result = ((uint16_t)(ADRESH << 8) | ADRESL); //concatenate high and low registers to get ADC result
    return adc_result;
}


uint8_t CONFIG_PWM_CCP3(void){
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


uint8_t CONFIG_TMR0_INTERRUPT(void){
    TMR0IE = 1; //enable TMR0 interrupts
    return 1;
}


uint8_t CONFIG_SYSTEM(void){
    CONFIG_INT_OSC();
    CONFIG_PORTS();
    CONFIG_ADC_PINS();
    CONFIG_PWM_CCP3();
    return 1;
}


uint8_t TURN_ON_CCP3_PWM(void){
    //this procedure ensures near as dammit that a the PWM output will start with a full cycle
    TMR2IF = 0; //make sure TMR2IF is cleared
    TMR2ON = 1; //turn on timer2
    while(TMR2IF == 0){} //wait for TMR2 to overflow
    TMR2IF = 0; //clear TMR2IF
    TRISA2 = 0; //set RA2 to output CCP3 PWM
    return 1;
}