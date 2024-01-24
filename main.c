/*
 * File:   main.c
 * Author: jamesgarvey
 *
 * Created on December 13, 2023, 4:40 PM
 */

// PIC16F1824 Configuration Bit Settings

// 'C' source line config statements

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
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <math.h>
#include <stdio.h>

/////Stuff for ASM Code////////////////////////////

//equate names for registers with the register addresses
        asm("r1 EQU 20h");
        asm("r2 EQU 21h");
        asm("r3 EQU 22h");
        asm("aL EQU 23h");
        asm("aH EQU 24h");
        asm("B EQU 25h");
        //asm("STATUS EQU 03h"); //don't think I need to do this as maybe STATUS is already equ in xc.h
//define macro mmac, you then can 'call' the macro, and the arguments in sequence get replaced with the parameters you specify
        asm("mmac MACRO A,bit,u2,u1");
        asm("BTFSC A,bit");
        asm("ADDWF u2,F"); //don't know what the Fs mean tbh, but they appear in the electric druid code so...?
        asm("RRF u2,F");
        asm("RRF u1,F");
        asm("ENDM");
uint16_t *top_two_bytes_ptr = (uint16_t *)0x21; //point to specific memory address

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// DEFINE WAVETABLES /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

const uint16_t sine_table_one_quadrant[129]={512,518,524,530,537,543,549,555,562,568,574,580,587,593,599,605,611,617,624,630,636,642,648,654,660,666,672,678,684,690,696,701,707,713,719,725,730,736,741,747,753,758,764,769,774,780,785,790,796,801,806,811,816,821,826,831,836,841,846,850,855,860,864,869,873,878,882,886,890,895,899,903,907,911,915,919,922,926,930,933,937,940,944,947,950,953,957,960,963,966,968,971,974,977,979,982,984,986,989,991,993,995,997,999,1001,1003,1004,1006,1008,1009,1011,1012,1013,1014,1015,1017,1017,1018,1019,1020,1021,1021,1022,1022,1022,1023,1023,1023,1023};
const uint16_t tri_table_one_quadrant[129]={512,516,520,524,528,532,536,540,544,548,552,556,560,564,568,572,576,580,584,588,592,596,600,604,608,612,616,620,624,628,632,636,640,644,648,652,656,660,664,668,672,676,680,684,688,692,696,700,704,708,712,716,720,724,728,732,736,740,744,748,752,756,760,763,767,771,775,779,783,787,791,795,799,803,807,811,815,819,823,827,831,835,839,843,847,851,855,859,863,867,871,875,879,883,887,891,895,899,903,907,911,915,919,923,927,931,935,939,943,947,951,955,959,963,967,971,975,979,983,987,991,995,999,1003,1007,1011,1015,1019,1023};
const uint8_t lengthen_period_log_table[129]={97,97,97,97,96,96,96,96,96,96,95,95,95,95,95,95,94,94,94,94,94,93,93,93,93,93,93,92,92,92,92,92,91,91,91,91,90,90,90,90,90,89,89,89,89,88,88,88,88,87,87,87,87,86,86,86,86,85,85,85,85,84,84,84,83,83,83,82,82,82,81,81,81,80,80,80,79,79,78,78,78,77,77,76,76,76,75,75,74,74,73,73,72,72,71,70,70,69,69,68,67,67,66,65,64,63,63,62,61,60,59,58,57,55,54,53,51,50,48,46,44,42,39,36,32,28,22,14,0};
//const uint8_t lengthen_period_log_table[129]={108,108,108,107,107,107,107,107,107,106,106,106,106,106,105,105,105,105,105,104,104,104,104,104,103,103,103,103,103,102,102,102,102,101,101,101,101,100,100,100,100,100,99,99,99,98,98,98,98,97,97,97,97,96,96,96,95,95,95,94,94,94,93,93,93,92,92,92,91,91,91,90,90,89,89,89,88,88,87,87,86,86,86,85,85,84,84,83,83,82,81,81,80,80,79,78,78,77,76,76,75,74,73,72,72,71,70,69,68,67,65,64,63,62,60,59,57,55,53,51,49,46,43,40,36,31,24,15,0};
//const uint8_t lengthen_period_log_table[129]={110,110,110,109,109,109,109,109,109,108,108,108,108,108,107,107,107,107,107,106,106,106,106,106,105,105,105,105,104,104,104,104,104,103,103,103,103,102,102,102,102,101,101,101,101,100,100,100,99,99,99,99,98,98,98,97,97,97,96,96,96,96,95,95,94,94,94,93,93,93,92,92,92,91,91,90,90,89,89,89,88,88,87,87,86,86,85,85,84,83,83,82,82,81,80,80,79,78,78,77,76,75,75,74,73,72,71,70,69,68,67,65,64,63,61,60,58,56,54,52,50,47,44,41,36,31,25,16,0};
//const uint8_t lengthen_period_log_table[129]={106,106,106,105,105,105,105,105,105,104,104,104,104,104,103,103,103,103,103,103,102,102,102,102,102,101,101,101,101,100,100,100,100,100,99,99,99,99,98,98,98,98,97,97,97,97,96,96,96,96,95,95,95,94,94,94,94,93,93,93,92,92,92,91,91,91,90,90,90,89,89,89,88,88,87,87,87,86,86,85,85,84,84,84,83,83,82,82,81,80,80,79,79,78,78,77,76,76,75,74,73,73,72,71,70,69,68,67,66,65,64,63,62,60,59,58,56,54,52,50,48,45,42,39,35,30,24,15,0};
const uint16_t shorten_period_log_table[129]={256,245,234,224,215,206,197,188,180,173,165,158,152,145,139,133,127,122,117,112,107,102,98,94,90,86,82,79,75,72,69,66,63,60,58,55,53,51,48,46,44,42,41,39,37,35,34,32,31,30,28,27,26,25,24,23,22,21,20,19,18,17,16,16,15,14,14,13,12,12,11,11,10,10,9,9,9,8,8,7,7,7,6,6,6,5,5,5,5,4,4,4,4,4,3,3,3,3,3,3,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0};
//const uint16_t prescaler[8] = {256,128,64,32,16,8,4,2};
const uint8_t prescaler_bits[8] = {0b111,0b110,0b101,0b100,0b011,0b010,0b001,0b000}; //we don't use 2x prescaler but here for completeness
const uint8_t waveshape_adc_config_value = 0b100;
const uint8_t speed_adc_config_value = 0b101;
const uint8_t depth_adc_config_value = 0b110;
const uint8_t symmetry_adc_config_value = 0b111;
const uint8_t POSITIVE = 1;
const uint8_t NEGATIVE = 0;
const uint8_t DO_NOTHING = 0;
const uint8_t DIVIDE_BY_TWO = 1;
const uint8_t MULTIPLY_BY_TWO = 2;
const uint8_t DIVIDE_BY_FOUR = 3;
const uint8_t DONT_CARE = 4;
const uint8_t YES = 1;
const uint8_t NO = 0;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// PRE-REQUISITES ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Wave selection pot is connected to RC0 (AN4) - modifier = 0
////////// If 0 <= ADC < 341 -> triangle selected
////////// If 341 <= ADC < 682 -> sine selected
////////// If 682 <= ADC < 1023 -> square selected
// Speed pot (or external treadle pot) is connected to RC1 (AN5) - modifier = 1
// Depth pot is connected to RC2 (AN6) - modifier = 2

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// DECLARE VARIABLES //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

volatile uint16_t speed_control;
volatile uint32_t speed_control_32;
volatile uint16_t how_many_128;
volatile uint16_t duty;
volatile uint8_t duty_low_byte;
volatile uint8_t duty_high_byte;
volatile uint8_t current_waveshape;
volatile uint16_t current_speed_linear;
volatile uint32_t current_speed_linear_32;
volatile uint16_t current_speed_log;
volatile uint32_t current_speed_log_32;
volatile uint16_t current_depth;
volatile uint16_t current_symmetry;
volatile uint8_t current_one_quadrant_index = 0;
volatile uint8_t current_halfcycle = 0;
volatile uint8_t current_quadrant = 0;
volatile uint8_t final_TMR0;
volatile uint8_t TMR0_offset;
volatile uint8_t TMR0_offset_sign;
volatile uint8_t prescaler_adjust;
volatile uint8_t raw_TMR0;
volatile uint8_t base_prescaler_bits_index;
volatile uint16_t dTMR0_ideal;
volatile uint8_t clear_TMR0_please;
volatile uint8_t symmetry_status;
volatile uint32_t symmetry_total;

#define MAX_QUADRANT_INDEX 128
#define MIN_QUADRANT_INDEX 0
#define _XTAL_FREQ 32000000
#define TRIANGLE_MODE 0
#define SINE_MODE 1
#define SQUARE_MODE 2
#define NUMBER_OF_FREQUENCY_STEPS 600; //883 in calcs, seems to be wrong, but 650 gives 15Hz max freq.
#define FIRST_HALFCYCLE 0
#define SECOND_HALFCYCLE 1
#define FIRST_QUADRANT 0
#define SECOND_QUADRANT 1
#define TRIANGLE_MODE_ADC_THRESHOLD 341
#define SINE_MODE_ADC_THRESHOLD 682
#define SQUARE_MODE_ADC_THRESHOLD 1023
#define CW 1
#define CCW 0
#define MAX_SYMMETRY_TOTAL 361
#define SHORTEN_PERIOD_FRACTION_16_BIT_NUMBER 47926
#define LENGTHEN_PERIOD_FRACTION_16_BIT_NUMBER 17609

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// DEFINE FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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

uint8_t DETERMINE_WAVESHAPE(){
    uint16_t ADC = DO_ADC(&waveshape_adc_config_value);
    if(ADC <= TRIANGLE_MODE_ADC_THRESHOLD){
            return TRIANGLE_MODE; //triangle wave
        }
        if (ADC <= SINE_MODE_ADC_THRESHOLD){
            return SINE_MODE; //sine wave
        }
        if (ADC <= SQUARE_MODE_ADC_THRESHOLD){
            return SQUARE_MODE; //square wave
        }
        else{
            return SINE_MODE; //if error, return sine
        }
    return 1;
}

uint8_t SET_DUTY_CCP3(volatile uint16_t *duty_ptr){
    //we need to split up the duty cycle value (0-1023) into two parts, the 8 MSBs (CCPR3L register) and the 2 LSBs (DC3B bits within the CCP3CON register) and write the separate bit portions to those registers
    CCPR3L = (uint8_t)(*duty_ptr >> 2); //extract 8 MSBs from duty value (dereference the ptr first)
    uint8_t temp = *duty_ptr % 0b100;
    CCP3CON = CCP3CON | ((uint8_t)(temp << 4)); //extract 2 LSBs from duty value and write to the DC3B bits (bits 5 and 4 within the CCP3CON register)
    //although we can write to the above registers at any time to specify the duty cycle, the duty cycle will not update until the
    //timer overflow
    return 1;
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

uint8_t TURN_ON_CCP3_PWM(void){
    //this procedure ensures near as dammit that a the PWM output will start with a full cycle
    TMR2IF = 0; //make sure TMR2IF is cleared
    TMR2ON = 1; //turn on timer2
    while(TMR2IF == 0){} //wait for TMR2 to overflow
    TMR2IF = 0; //clear TMR2IF
    TRISA2 = 0; //set RA2 to output CCP3 PWM
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

uint8_t TMR0_WRITE(volatile uint8_t *timer_value_ptr){
    TMR0 = (uint8_t) *timer_value_ptr; //recast as 8-bit
    return 1;
}
    
uint8_t GET_CURRENT_POT_VALUES(void){
    current_waveshape = DETERMINE_WAVESHAPE();
    current_speed_linear = DO_ADC(&speed_adc_config_value); //get speed (10-bit linear)
    current_depth = DO_ADC(&depth_adc_config_value); //get depth (10-bit linear)
    current_depth = current_depth >> 2; //convert to 8-bit
    current_symmetry = DO_ADC(&symmetry_adc_config_value); //get symmetry (10-bit linear)
    current_symmetry = current_symmetry >> 2; //convert to 8-bit
    return 1;
}

uint8_t PROCESS_RAW_SPEED_AND_PRESCALER(void){
    current_speed_linear_32 = current_speed_linear;
    speed_control_32 = current_speed_linear_32 * NUMBER_OF_FREQUENCY_STEPS;
    speed_control_32 = speed_control_32 >> 10;
    speed_control = (uint16_t) speed_control_32;
    //speed_control = (speed_adc_10_bit/1024)*883
        if(speed_control <= (127-12)){ //inequality is correct!
            raw_TMR0 = (uint8_t) speed_control + 12; //set TMR0
            base_prescaler_bits_index = 1;
        }
        else{ 	//(speed_control > (127-12))
            uint16_t speed_control_subtracted;
            speed_control_subtracted = speed_control - (127-12);
            how_many_128 = (uint8_t)(speed_control_subtracted >> 7); //divide by 128, i.e. return how many 128s go into the speed_control
            raw_TMR0 = (uint8_t) (speed_control_subtracted - (how_many_128 << 7)); //how_many_128*128, set TMR0
            base_prescaler_bits_index = (uint8_t)(how_many_128 + 2);
        }
    return 1;
}

uint8_t PROCESS_FINAL_SPEED_AND_PRESCALER(void){
    if(clear_TMR0_please){
        raw_TMR0 = 0;
    }
    if((TMR0_offset_sign == POSITIVE) || (TMR0_offset_sign == DONT_CARE)){
        final_TMR0 = raw_TMR0 + TMR0_offset;
    }
    else if(TMR0_offset_sign == NEGATIVE){
        final_TMR0 = raw_TMR0 - TMR0_offset;
    }
    if(prescaler_adjust == DIVIDE_BY_TWO){
        OPTION_REG = prescaler_bits[base_prescaler_bits_index + 1];
    }
    else if(prescaler_adjust == DIVIDE_BY_FOUR){
        OPTION_REG = prescaler_bits[base_prescaler_bits_index + 2];
    }
    else if(prescaler_adjust == MULTIPLY_BY_TWO){
        OPTION_REG = prescaler_bits[base_prescaler_bits_index - 1];
    }
    else if(prescaler_adjust == DO_NOTHING){
        OPTION_REG = prescaler_bits[base_prescaler_bits_index];
    }
    return 1;
}   

uint8_t SHORTEN_PERIOD(void){
    uint16_t dTMR0_ideal = (uint16_t)((uint32_t)(symmetry_total * SHORTEN_PERIOD_FRACTION_16_BIT_NUMBER) >> 16);

    if((dTMR0_ideal + raw_TMR0) < 128){
        TMR0_offset = (uint8_t)dTMR0_ideal;
        TMR0_offset_sign = POSITIVE;
        prescaler_adjust = DO_NOTHING;
        clear_TMR0_please = NO;
    }
    else if((dTMR0_ideal + raw_TMR0) == 128){
        TMR0_offset = 0;
        TMR0_offset_sign = DONT_CARE;
        prescaler_adjust = DIVIDE_BY_TWO;
        clear_TMR0_please = YES;
    }
    else if((dTMR0_ideal + raw_TMR0) > 128){
        uint16_t TMR0_raw_extra = dTMR0_ideal + raw_TMR0 - 128;
        if(TMR0_raw_extra >> 7 == 0){ //divide by 128
            TMR0_offset = (uint8_t)TMR0_raw_extra;
            TMR0_offset_sign = POSITIVE;
            prescaler_adjust = DIVIDE_BY_TWO;
            clear_TMR0_please = YES;
        }
        else if(TMR0_raw_extra >> 7 == 1){ //divide by 128
            TMR0_raw_extra = TMR0_raw_extra - 128;
            TMR0_offset = (uint8_t)TMR0_raw_extra;
            TMR0_offset_sign = POSITIVE;
            prescaler_adjust = DIVIDE_BY_FOUR;
            clear_TMR0_please = YES;
        }
    }
    return 1;
}   

uint8_t LENGTHEN_PERIOD(void){
    uint16_t dTMR0_ideal = (uint16_t)((uint32_t)(symmetry_total * LENGTHEN_PERIOD_FRACTION_16_BIT_NUMBER) >> 16);
    
        if(raw_TMR0 < dTMR0_ideal){
            TMR0_offset = (uint8_t)(128 - (dTMR0_ideal - raw_TMR0));
            TMR0_offset_sign = POSITIVE;                
            prescaler_adjust = MULTIPLY_BY_TWO;
            clear_TMR0_please = YES;
        }
        else{ //TMR0 >= dTMR0_ideal
            TMR0_offset = (uint8_t)dTMR0_ideal;
            TMR0_offset_sign = NEGATIVE;
            prescaler_adjust = DO_NOTHING;
            clear_TMR0_please = NO;
        }
    return 1;
}

uint8_t PROCESS_TMR0_OFFSET_AND_PRESCALER_ADJUST(void){
    if(current_symmetry == 128){
        TMR0_offset = 0;
        TMR0_offset_sign = POSITIVE;
        prescaler_adjust = DO_NOTHING;
        clear_TMR0_please = NO;
        return 1; //exit function
    }
    uint8_t symmetry_status = CCW;
    if(current_symmetry > 128){
        current_symmetry = 255 - current_symmetry; //converts current_symmetry to 128 -> 0 range (same range as CCW regime, more or less)
        symmetry_status = CW;
    }
    
    uint16_t temp = (uint16_t)(MAX_SYMMETRY_TOTAL * (128 - current_symmetry));
    symmetry_total = (uint32_t)(temp >> 7);

    if((current_halfcycle == FIRST_HALFCYCLE) && (symmetry_status == CCW)){
        SHORTEN_PERIOD();
    }
    else if((current_halfcycle == FIRST_HALFCYCLE) && (symmetry_status == CW)){
        LENGTHEN_PERIOD();
    }
    else if((current_halfcycle == SECOND_HALFCYCLE) && (symmetry_status == CCW)){
        LENGTHEN_PERIOD();
    }    
    else if((current_halfcycle == SECOND_HALFCYCLE) && (symmetry_status == CW)){
        SHORTEN_PERIOD();
    }
    return 1;
}
    
void __interrupt() INTERRUPT_InterruptManager(void){
    if(TMR0IF == 1){
    GIE = 0; //disable interrupts
    TMR0 = final_TMR0; //this line must go here!
    LATC5 = 1; //start ISR length measurement
    TMR0IF = 0; //clear TMR0 interrupt flag
    
    if(current_waveshape == TRIANGLE_MODE){
        duty = tri_table_one_quadrant[current_one_quadrant_index];
    }
    else if(current_waveshape == SINE_MODE){
        duty = sine_table_one_quadrant[current_one_quadrant_index];
    }
    else if(current_waveshape == SQUARE_MODE){
        duty = 1023;
    }
    if(current_one_quadrant_index == MAX_QUADRANT_INDEX){
        current_quadrant = SECOND_QUADRANT;
    }
    if(current_one_quadrant_index == MIN_QUADRANT_INDEX){
        current_quadrant = FIRST_QUADRANT;
        if(current_halfcycle == FIRST_HALFCYCLE){
            current_halfcycle = SECOND_HALFCYCLE;
        }
        else{
            current_halfcycle = FIRST_HALFCYCLE;
        }
    }
        if(current_quadrant == FIRST_QUADRANT){
            current_one_quadrant_index++;
        }
        else{
            current_one_quadrant_index--;
        }
        if(current_halfcycle == SECOND_HALFCYCLE){
        duty = 1023 - duty;
        }
    
    //Apply Depth
    if(current_depth != 0){
        duty_low_byte = duty & 0xFF;
        duty_high_byte = duty >> 8;
        
        //////////////////////////////////////////////////////////////////////////////////
        //multiply 16x8 in assembly. Copied from 16x8 multiply algorithm by Martin Sturm//
        //////////////////////////////////////////////////////////////////////////////////
        
        ///See equates outside of ISR/////////////////////////////////////////////////////

        asm("CLRF r3");
        asm("CLRF r1");
        asm("BCF STATUS,0"); //a.k.a CLRC - clear carry bit
        asm("MOVF _current_depth,0"); //move B to W //comment out if 8bit multiplicand already in W
        //also, b can be removed from macro arguments
        asm("mmac _duty_low_byte,0,r3,r1");
        asm("mmac _duty_low_byte,1,r3,r1");
        asm("mmac _duty_low_byte,2,r3,r1");
        asm("mmac _duty_low_byte,3,r3,r1");
        asm("mmac _duty_low_byte,4,r3,r1");
        asm("mmac _duty_low_byte,5,r3,r1");
        asm("mmac _duty_low_byte,6,r3,r1");
        asm("mmac _duty_low_byte,7,r3,r1");
        
        asm("CLRF r2");
        //carry already cleared from last RRF of mmac above
        //8bit multiplicand still in W
        asm("mmac _duty_high_byte,0,r3,r2");
        asm("mmac _duty_high_byte,1,r3,r2");
        asm("mmac _duty_high_byte,2,r3,r2");
        asm("mmac _duty_high_byte,3,r3,r2");
        asm("mmac _duty_high_byte,4,r3,r2");
        asm("mmac _duty_high_byte,5,r3,r2");
        asm("mmac _duty_high_byte,6,r3,r2");
        asm("mmac _duty_high_byte,7,r3,r2");
        
        //by accessing only the top and middle bytes of the 24-bit result, we also divide by 256. So end result is duty = 1023 - duty*(current_depth/256)
        duty = 1023 - *top_two_bytes_ptr;
    }
    else{
        duty = 1023; //if depth is 0, just output 1023
    }
 
    //Write Duty
    SET_DUTY_CCP3(&duty);
    
    //Finish Up
    LATC5 = 0; //finish ISR length measurement
    GIE = 1;
    }
}

void main(void) {
    
    CONFIG_SYSTEM();
    TURN_ON_CCP3_PWM();
    CONFIG_TMR0_INTERRUPT();
    GET_CURRENT_POT_VALUES();
    PROCESS_RAW_SPEED_AND_PRESCALER();
    PROCESS_TMR0_OFFSET_AND_PRESCALER_ADJUST();
    PROCESS_FINAL_SPEED_AND_PRESCALER();
    TMR0_WRITE(&final_TMR0);
    GIE = 1; //enable interrupts
    
    while(1){ //infinite loop
        //__delay_ms(1);
        GET_CURRENT_POT_VALUES();
        PROCESS_RAW_SPEED_AND_PRESCALER();
        PROCESS_TMR0_OFFSET_AND_PRESCALER_ADJUST();
        PROCESS_FINAL_SPEED_AND_PRESCALER();
        }
}
       
       
