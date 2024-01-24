#ifndef SYSTEM_H
    #define SYSTEM_H

    #include <xc.h>
    #include <stdio.h>
    #include <math.h>

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
    #define RESOLUTION_OF_TOTAL_SYMMETRY_FRACTION 16
    #define SYMMETRY_ADC_RESOLUTION 8
    #define SYMMETRY_ON_OR_OFF 0
    #define DEPTH_ON_OR_OFF 0


    #if SYMMETRY_ADC_RESOLUTION == 10
        #define SYMMETRY_ADC_HALF_SCALE_NO_BITS 9
        #define SYMMETRY_ADC_FULL_SCALE 1023
        #define SYMMETRY_ADC_HALF_SCALE 512
    #endif


    #if SYMMETRY_ADC_RESOLUTION == 8
        #define SYMMETRY_ADC_HALF_SCALE_NO_BITS 7
        #define SYMMETRY_ADC_FULL_SCALE 255
        #define SYMMETRY_ADC_HALF_SCALE 128
    #endif


    #if DEPTH_ON_OR_OFF == 1
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
    #endif

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

    uint8_t DETERMINE_WAVESHAPE(void);
    uint8_t SET_DUTY_CCP3(volatile uint16_t *duty_ptr);
    uint8_t GET_CURRENT_POT_VALUES(void);
    uint8_t PROCESS_RAW_SPEED_AND_PRESCALER(void);
    uint8_t CHECK_IF_PRESCALER_NEEDS_TO_BE_1_1(void);
    uint8_t TURN_PRESCALER_OFF(void);
    uint8_t TURN_PRESCALER_ON(void);
    uint8_t PROCESS_FINAL_SPEED_AND_PRESCALER(void);
    uint8_t ADJUST_TMR0(void);
    uint8_t ADJUST_PRESCALER(void);
    uint8_t SHORTEN_PERIOD(void);
    uint8_t LENGTHEN_PERIOD(void);
    uint8_t PROCESS_TMR0_OFFSET_AND_PRESCALER_ADJUST(void);
    uint16_t DO_ADC(const uint8_t *waveshape_adc_config_value);

    volatile extern uint8_t final_TMR0;
    volatile extern uint8_t TMR0_offset;
    volatile extern uint8_t TMR0_offset_sign;
    volatile extern uint8_t prescaler_adjust;
    volatile extern uint8_t raw_TMR0;
    volatile extern uint8_t base_prescaler_bits_index;
    volatile extern uint16_t dTMR0_ideal;
    volatile extern uint8_t clear_TMR0_please;
    volatile extern uint8_t symmetry_status;
    volatile extern uint32_t symmetry_total;
    volatile extern uint16_t speed_control;
    volatile extern uint32_t speed_control_32;
    volatile extern uint8_t how_many_128;
    volatile extern uint16_t duty;
    volatile extern uint8_t duty_low_byte;
    volatile extern uint8_t duty_high_byte;
    volatile extern uint8_t current_waveshape;
    volatile extern uint16_t current_speed_linear;
    volatile extern uint32_t current_speed_linear_32;
    volatile extern uint16_t current_depth;
    volatile extern uint16_t current_symmetry;
    volatile extern uint8_t current_one_quadrant_index;
    volatile extern uint8_t current_halfcycle;
    volatile extern uint8_t current_quadrant;

#endif


