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
//const uint8_t lin_to_log_8_bit[1024]={0,4,29,44,54,62,69,74,79,84,87,91,94,97,100,102,104,107,109,111,112,114,116,118,119,121,122,123,125,126,127,128,130,131,132,133,134,135,136,137,138,138,139,140,141,142,143,143,144,145,146,146,147,148,148,149,150,150,151,152,152,153,153,154,155,155,156,156,157,157,158,158,159,159,160,160,161,161,162,162,163,163,164,164,164,165,165,166,166,167,167,167,168,168,169,169,169,170,170,170,171,171,172,172,172,173,173,173,174,174,174,175,175,175,176,176,176,176,177,177,177,178,178,178,179,179,179,179,180,180,180,181,181,181,181,182,182,182,182,183,183,183,183,184,184,184,184,185,185,185,185,186,186,186,186,187,187,187,187,188,188,188,188,188,189,189,189,189,190,190,190,190,190,191,191,191,191,191,192,192,192,192,192,193,193,193,193,193,194,194,194,194,194,195,195,195,195,195,196,196,196,196,196,196,197,197,197,197,197,197,198,198,198,198,198,199,199,199,199,199,199,200,200,200,200,200,200,200,201,201,201,201,201,201,202,202,202,202,202,202,202,203,203,203,203,203,203,204,204,204,204,204,204,204,205,205,205,205,205,205,205,206,206,206,206,206,206,206,206,207,207,207,207,207,207,207,208,208,208,208,208,208,208,208,209,209,209,209,209,209,209,209,210,210,210,210,210,210,210,210,211,211,211,211,211,211,211,211,212,212,212,212,212,212,212,212,212,213,213,213,213,213,213,213,213,213,214,214,214,214,214,214,214,214,214,215,215,215,215,215,215,215,215,215,216,216,216,216,216,216,216,216,216,216,217,217,217,217,217,217,217,217,217,217,218,218,218,218,218,218,218,218,218,218,219,219,219,219,219,219,219,219,219,219,220,220,220,220,220,220,220,220,220,220,220,221,221,221,221,221,221,221,221,221,221,221,222,222,222,222,222,222,222,222,222,222,222,222,223,223,223,223,223,223,223,223,223,223,223,224,224,224,224,224,224,224,224,224,224,224,224,225,225,225,225,225,225,225,225,225,225,225,225,225,226,226,226,226,226,226,226,226,226,226,226,226,227,227,227,227,227,227,227,227,227,227,227,227,227,228,228,228,228,228,228,228,228,228,228,228,228,228,228,229,229,229,229,229,229,229,229,229,229,229,229,229,230,230,230,230,230,230,230,230,230,230,230,230,230,230,230,231,231,231,231,231,231,231,231,231,231,231,231,231,231,232,232,232,232,232,232,232,232,232,232,232,232,232,232,232,233,233,233,233,233,233,233,233,233,233,233,233,233,233,233,233,234,234,234,234,234,234,234,234,234,234,234,234,234,234,234,235,235,235,235,235,235,235,235,235,235,235,235,235,235,235,235,235,236,236,236,236,236,236,236,236,236,236,236,236,236,236,236,236,237,237,237,237,237,237,237,237,237,237,237,237,237,237,237,237,237,238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,238,239,239,239,239,239,239,239,239,239,239,239,239,239,239,239,239,239,239,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,240,241,241,241,241,241,241,241,241,241,241,241,241,241,241,241,241,241,241,241,242,242,242,242,242,242,242,242,242,242,242,242,242,242,242,242,242,242,242,242,243,243,243,243,243,243,243,243,243,243,243,243,243,243,243,243,243,243,243,243,244,244,244,244,244,244,244,244,244,244,244,244,244,244,244,244,244,244,244,244,244,245,245,245,245,245,245,245,245,245,245,245,245,245,245,245,245,245,245,245,245,245,246,246,246,246,246,246,246,246,246,246,246,246,246,246,246,246,246,246,246,246,246,246,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,247,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,248,249,249,249,249,249,249,249,249,249,249,249,249,249,249,249,249,249,249,249,249,249,249,249,249,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,250,251,251,251,251,251,251,251,251,251,251,251,251,251,251,251,251,251,251,251,251,251,251,251,251,251,252,252,252,252,252,252,252,252,252,252,252,252,252,252,252,252,252,252,252,252,252,252,252,252,252,252,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,253,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,254,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255};
//const uint16_t lin_to_log_10_bit[1024]={0,102,162,205,238,264,287,307,324,340,354,367,379,390,400,409,418,427,435,442,449,456,463,469,475,481,486,492,497,502,507,512,516,521,525,529,533,537,541,545,548,552,555,559,562,565,568,571,574,577,580,583,586,589,592,594,597,599,602,604,607,609,612,614,616,618,621,623,625,627,629,631,633,635,637,639,641,643,645,647,649,650,652,654,656,657,659,661,663,664,666,667,669,671,672,674,675,677,678,680,681,683,684,686,687,688,690,691,692,694,695,696,698,699,700,702,703,704,705,707,708,709,710,712,713,714,715,716,717,718,720,721,722,723,724,725,726,727,728,729,730,732,733,734,735,736,737,738,739,740,741,742,743,743,744,745,746,747,748,749,750,751,752,753,754,755,755,756,757,758,759,760,761,762,762,763,764,765,766,767,767,768,769,770,771,771,772,773,774,775,775,776,777,778,778,779,780,781,781,782,783,784,784,785,786,786,787,788,789,789,790,791,791,792,793,793,794,795,795,796,797,797,798,799,799,800,801,801,802,803,803,804,805,805,806,807,807,808,808,809,810,810,811,811,812,813,813,814,814,815,816,816,817,817,818,819,819,820,820,821,821,822,822,823,824,824,825,825,826,826,827,827,828,829,829,830,830,831,831,832,832,833,833,834,834,835,835,836,836,837,837,838,838,839,839,840,840,841,841,842,842,843,843,844,844,845,845,846,846,847,847,848,848,849,849,850,850,851,851,851,852,852,853,853,854,854,855,855,856,856,856,857,857,858,858,859,859,860,860,860,861,861,862,862,863,863,863,864,864,865,865,866,866,866,867,867,868,868,868,869,869,870,870,870,871,871,872,872,872,873,873,874,874,874,875,875,876,876,876,877,877,878,878,878,879,879,880,880,880,881,881,881,882,882,883,883,883,884,884,884,885,885,885,886,886,887,887,887,888,888,888,889,889,889,890,890,891,891,891,892,892,892,893,893,893,894,894,894,895,895,895,896,896,896,897,897,897,898,898,898,899,899,899,900,900,900,901,901,901,902,902,902,903,903,903,904,904,904,905,905,905,906,906,906,907,907,907,908,908,908,909,909,909,909,910,910,910,911,911,911,912,912,912,913,913,913,913,914,914,914,915,915,915,916,916,916,916,917,917,917,918,918,918,919,919,919,919,920,920,920,921,921,921,921,922,922,922,923,923,923,923,924,924,924,925,925,925,925,926,926,926,926,927,927,927,928,928,928,928,929,929,929,930,930,930,930,931,931,931,931,932,932,932,932,933,933,933,934,934,934,934,935,935,935,935,936,936,936,936,937,937,937,937,938,938,938,938,939,939,939,939,940,940,940,941,941,941,941,942,942,942,942,943,943,943,943,944,944,944,944,944,945,945,945,945,946,946,946,946,947,947,947,947,948,948,948,948,949,949,949,949,950,950,950,950,951,951,951,951,951,952,952,952,952,953,953,953,953,954,954,954,954,954,955,955,955,955,956,956,956,956,957,957,957,957,957,958,958,958,958,959,959,959,959,959,960,960,960,960,961,961,961,961,961,962,962,962,962,962,963,963,963,963,964,964,964,964,964,965,965,965,965,966,966,966,966,966,967,967,967,967,967,968,968,968,968,968,969,969,969,969,970,970,970,970,970,971,971,971,971,971,972,972,972,972,972,973,973,973,973,973,974,974,974,974,974,975,975,975,975,975,976,976,976,976,976,977,977,977,977,977,978,978,978,978,978,979,979,979,979,979,980,980,980,980,980,980,981,981,981,981,981,982,982,982,982,982,983,983,983,983,983,984,984,984,984,984,984,985,985,985,985,985,986,986,986,986,986,987,987,987,987,987,987,988,988,988,988,988,989,989,989,989,989,989,990,990,990,990,990,991,991,991,991,991,991,992,992,992,992,992,992,993,993,993,993,993,994,994,994,994,994,994,995,995,995,995,995,995,996,996,996,996,996,997,997,997,997,997,997,998,998,998,998,998,998,999,999,999,999,999,999,1000,1000,1000,1000,1000,1000,1001,1001,1001,1001,1001,1001,1002,1002,1002,1002,1002,1002,1003,1003,1003,1003,1003,1003,1004,1004,1004,1004,1004,1004,1005,1005,1005,1005,1005,1005,1006,1006,1006,1006,1006,1006,1007,1007,1007,1007,1007,1007,1007,1008,1008,1008,1008,1008,1008,1009,1009,1009,1009,1009,1009,1010,1010,1010,1010,1010,1010,1011,1011,1011,1011,1011,1011,1011,1012,1012,1012,1012,1012,1012,1013,1013,1013,1013,1013,1013,1013,1014,1014,1014,1014,1014,1014,1015,1015,1015,1015,1015,1015,1015,1016,1016,1016,1016,1016,1016,1017,1017,1017,1017,1017,1017,1017,1018,1018,1018,1018,1018,1018,1018,1019,1019,1019,1019,1019,1019,1019,1020,1020,1020,1020,1020,1020,1021,1021,1021,1021,1021,1021,1021,1022,1022,1022,1022,1022,1022,1022,1023,1023,1023,1023};
//const uint16_t prescaler[8] = {256,128,64,32,16,8,4,2}; //we don?t use 2 but here for completeness
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
volatile uint16_t speed_control_subtracted;
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

#define MAX_QUADRANT_INDEX 128
#define MIN_QUADRANT_INDEX 0
#define _XTAL_FREQ 32000000
#define TRIANGLE_MODE 0
#define SINE_MODE 1
#define SQUARE_MODE 2
#define NUMBER_OF_FREQUENCY_STEPS 650; //883 in calcs, seems to be wrong, but 650 gives 15Hz max freq.
#define FIRST_HALFCYCLE 0
#define SECOND_HALFCYCLE 1
#define FIRST_QUADRANT 0
#define SECOND_QUADRANT 1
#define TRIANGLE_MODE_ADC_THRESHOLD 341
#define SINE_MODE_ADC_THRESHOLD 682
#define SQUARE_MODE_ADC_THRESHOLD 1023

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////// DEFINE FUNCTIONS //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CONFIG_INT_OSC(void){
    OSCCON = 0b11110000; //defer clock selection to configuration word 1, also select 32MHz as the system clock, and turn on PLL. 
    OSCTUNE = 0b00011111; //run oscillator as fast as possible
    //should select 32MHz
}

void CONFIG_PORTS(void){
    ANSELC = 0b00001111; //set RC0-3 to analog inputs, RC5 as output
    TRISC = 0b00001111; //set RC0-3 to inputs, RC5 as output
    WPUC = 0b00000000; //disable pullups on port C
}

void CONFIG_ADC_PINS(void){
    ADCON1 = 0b10100000; //set ADC references to VDD and VSS, choose ADC clock source to be system_clock/32, and right-justify the ADC result registers.
}

uint16_t DO_ADC(const uint8_t *modifier){
    ADCON0 = 0x00; //clear ADCON0 first
    //uint8_t temp = A2D_lookup[*modifier];
    uint8_t temp = *modifier << 2;
    ADCON0 = ADCON0 | temp; //select appropriate ADC channel
    ADON = 1; //turn on ADC
    __delay_ms(0.005); //acquisition time
    GO_nDONE = 1; //start A2D
    while(GO_nDONE == 1){} //do nothing while ADC conversion in progress
    ADON = 0; //turn off ADC
    uint16_t adc_result = ((ADRESH << 8) | ADRESL); //concatenate high and low registers to get ADC result
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
}

/*void CONVERT_SPEED_TO_LOG(uint16_t *linear_speed_ptr, uint16_t *log_speed_ptr){ //from the speed pot, determine the duty cycle
    //both the ADC result and duty cycle are 10-bit so we don't have to convert between the two of them, woop,
    //but we do have to convert the linear ADC result into log-taper for the duty
    *log_speed_ptr = lin_to_log_10_bit[*linear_speed_ptr];
}*/

void SET_DUTY_CCP3(volatile uint16_t *duty_ptr){
    //we need to split up the duty cycle value (0-1023) into two parts, the 8 MSBs (CCPR3L register) and the 2 LSBs (DC3B bits within the CCP3CON register) and write the separate bit portions to those registers
    CCPR3L = *duty_ptr >> 2; //extract 8 MSBs from duty value (dereference the ptr first)
    uint8_t temp = *duty_ptr % 0b100;
    CCP3CON = CCP3CON | (temp << 4); //extract 2 LSBs from duty value and write to the DC3B bits (bits 5 and 4 within the CCP3CON register)
    //although we can write to the above registers at any time to specify the duty cycle, the duty cycle will not update until the
    //timer overflow
}

void CONFIG_PWM_CCP3(void){
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
}

void TURN_ON_CCP3_PWM(void){
    //this procedure ensures near as dammit that a the PWM output will start with a full cycle
    TMR2IF = 0; //make sure TMR2IF is cleared
    TMR2ON = 1; //turn on timer2
    while(TMR2IF == 0){} //wait for TMR2 to overflow
    TMR2IF = 0; //clear TMR2IF
    TRISA2 = 0; //set RA2 to output CCP3 PWM
}

/*void CONFIG_TMR0(void){
    //OPTION_REG = 0b00000101; //select prescaler and f_system/4 for clock source 
    OPTION_REG = 0b00000111; //select prescaler and f_system/4 for clock source
}*/

void CONFIG_TMR0_INTERRUPT(void){
    TMR0IE = 1; //enable TMR0 interrupts
}

void CONFIG_SYSTEM(void){
    CONFIG_INT_OSC();
    CONFIG_PORTS();
    CONFIG_ADC_PINS();
    CONFIG_PWM_CCP3();
}

void TMR0_WRITE(volatile uint8_t *timer_value_ptr){
    TMR0 = (uint8_t) *timer_value_ptr; //recast as 8-bit
}
    
void GET_CURRENT_POT_VALUES(void){
    current_waveshape = DETERMINE_WAVESHAPE();
    current_speed_linear = DO_ADC(&speed_adc_config_value); //get speed (10-bit linear)
    current_depth = DO_ADC(&depth_adc_config_value); //get depth (10-bit linear)
    current_depth = current_depth >> 2; //convert to 8-bit
    current_symmetry = DO_ADC(&symmetry_adc_config_value); //get symmetry (10-bit linear)
    current_symmetry = current_symmetry >> 2; //convert to 8-bit
}

void PROCESS_RAW_SPEED_AND_PRESCALER(void){
    current_speed_linear_32 = current_speed_linear;
    speed_control_32 = current_speed_linear_32 * NUMBER_OF_FREQUENCY_STEPS;
    speed_control_32 = speed_control_32 >> 10;
    speed_control = (uint16_t) speed_control_32;
    //speed_control = (speed_adc_10_bit/1024)*883
        if(speed_control <= (127-12)){ //inequality is correct!
            raw_TMR0 = (uint8_t) speed_control + 12; //set TMR0
            //OPTION_REG = prescaler_bits[1]; //set prescaler of TMR0
            base_prescaler_bits_index = 1;
        }
        else{ 	//(speed_control > (127-12))
            speed_control_subtracted = speed_control - (127-12);
            how_many_128 = speed_control_subtracted >> 7; //divide by 128, i.e. return how many 128s go into the speed_control
            raw_TMR0 = (uint8_t) (speed_control_subtracted - (how_many_128 << 7)); //how_many_128*128, set TMR0
            //OPTION_REG = prescaler_bits[(how_many_128 + 2)]; //set prescaler of TMR0
            base_prescaler_bits_index = how_many_128 + 2;
        }
}

void PROCESS_FINAL_SPEED_AND_PRESCALER(void){
    if(TMR0_offset_sign == POSITIVE){
        final_TMR0 = raw_TMR0 + TMR0_offset;
    }
    else if(TMR0_offset_sign == NEGATIVE){
        final_TMR0 = raw_TMR0 - TMR0_offset;
    }
    else if(TMR0_offset_sign == DO_NOTHING){
        final_TMR0 = raw_TMR0;
    }
    if(prescaler_adjust == DIVIDE_BY_TWO){
        OPTION_REG = prescaler_bits[base_prescaler_bits_index + 1];
    }
    else if(prescaler_adjust == MULTIPLY_BY_TWO){
        OPTION_REG = prescaler_bits[base_prescaler_bits_index - 1];
    }
    else if(prescaler_adjust == DO_NOTHING){
        OPTION_REG = prescaler_bits[base_prescaler_bits_index];
    }
}   

void PROCESS_TMR0_OFFSET_AND_PRESCALER_ADJUST(void){
    if(current_symmetry <= 127){ //no need to compare with greater than or equal to 0, as int in unsigned!
        if(current_halfcycle == FIRST_HALFCYCLE){
            if(current_symmetry == 0){
                TMR0_offset = 0;
                TMR0_offset_sign = POSITIVE;
                prescaler_adjust = DIVIDE_BY_TWO;
            }
            else{
                if(raw_TMR0 + (128 - current_symmetry) >= 128){
                    TMR0_offset = current_symmetry;
                    TMR0_offset_sign = NEGATIVE;
                    prescaler_adjust = DIVIDE_BY_TWO;
                }
                else{ //if it is not >=128, then it must be <=127
                    TMR0_offset = 128 - current_symmetry;
                    TMR0_offset_sign = POSITIVE;
                    prescaler_adjust = DO_NOTHING;
                }
            }
        }
        else{ //second halfcycle
            uint8_t y = 64 - (current_symmetry >> 1); //current_symmetry/2
            if(raw_TMR0 < y){
                TMR0_offset = 128 - y;
                TMR0_offset_sign = POSITIVE;
                prescaler_adjust = MULTIPLY_BY_TWO;
            }
            else{ //TMR0 >= y
                TMR0_offset = y;
                TMR0_offset_sign = NEGATIVE;
                prescaler_adjust = DO_NOTHING;
            }
        }
    }
    
    else if(current_symmetry >= 129){ //it will be less than or equal to 255, so no need to check
        if(current_halfcycle == FIRST_HALFCYCLE){
            uint8_t y = (current_symmetry >> 1) - 63; //current_symmetry/2
            if(raw_TMR0 < y){
                TMR0_offset = 128 - y;
                TMR0_offset_sign = POSITIVE;
                prescaler_adjust = MULTIPLY_BY_TWO;
            }
            else{ //TMR0 >= y
                TMR0_offset = y;
                TMR0_offset_sign = NEGATIVE;
                prescaler_adjust = DO_NOTHING;
            }
        }
        else{ //second halfcycle
            if(current_symmetry == 255){
                TMR0_offset = 0;
                TMR0_offset_sign = POSITIVE;
                prescaler_adjust = DIVIDE_BY_TWO;
            }
            else{
                if(raw_TMR0 + (current_symmetry - 127) >= 128){
                    TMR0_offset = 255 - current_symmetry;
                    TMR0_offset_sign = NEGATIVE;
                    prescaler_adjust = DIVIDE_BY_TWO;
                }
                else{ //if it is not >=128, then it must be <=127
                    TMR0_offset = current_symmetry - 127;
                    TMR0_offset_sign = POSITIVE;
                    prescaler_adjust = DO_NOTHING;
                    
                    //THIS THING IS SORT OF WORKING!!!! I WOULD CHECK THE LEGITIMACY OF THE INCREASING PERIOD REGIMES, AS THE FREQUERNCY OF THE WHOLE WAVE INCREASES IN THE MIDDLE OF SYMMETRY AND GOES BACK TO THE RIGHT FREQUENCY AT THE EXTREMES (ON BOTH SIDES)
                }
            }
        }
    }

    else if(current_symmetry == 128){
        TMR0_offset = 0;
        TMR0_offset_sign = POSITIVE;
        prescaler_adjust = DO_NOTHING;
    }

}
    
void __interrupt() INTERRUPT_InterruptManager(void){
    if(TMR0IF == 1){
    GIE = 0; //disable interrupts
    //TMR0_WRITE
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
       
       
