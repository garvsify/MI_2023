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
    #define SHORTEN_POWER_OF_TWO_CONSTANT_8_BIT_SYM 1024
    #define SHORTEN_POWER_OF_TWO_DIVISOR_8_BIT_SYM 12
    #define SHORTEN_POWER_OF_TWO_CONSTANT_10_BIT_SYM 4096
    #define SHORTEN_POWER_OF_TWO_DIVISOR_10_BIT_SYM 14
    #define LENGTHEN_CONSTANT_8_BIT_SYM 896
    #define LENGTHEN_POWER_OF_TWO_DIVISOR_8_BIT_SYM 9
    #define LENGTHEN_CONSTANT_10_BIT_SYM 3584
    #define LENGTHEN_POWER_OF_TWO_DIVISOR_10_BIT_SYM 11
    #define TENBITMINUSONE 1023
    #define EIGHTBITMINUSONE 255
    #define ON 1
    #define OFF 0

    #define SYMMETRY_ADC_RESOLUTION 8
    #define SYMMETRY_ON_OR_OFF ON
    #define DEPTH_ON_OR_OFF ON


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


    const uint8_t prescaler_bits[8] = {0b111,0b110,0b101,0b100,0b011,0b010,0b001,0b000}; //256,128,64,32,16,8,4,2
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

    
    uint8_t determine_waveshape(void);
    uint8_t set_duty_CCP3(volatile uint16_t *duty_ptr);
    uint8_t get_current_pot_values(void);
    uint8_t process_raw_speed_and_prescaler(void);
    uint8_t check_if_prescaler_needs_to_be_1_1(void);
    uint8_t turn_prescaler_OFF(void);
    uint8_t turn_prescaler_ON(void);
    uint8_t adjust_TMR0(void);
    uint8_t adjust_and_set_prescaler(void);
    uint8_t shorten_period(void);
    uint8_t lengthen_period(void);
    uint8_t process_TMR0_and_prescaler_adjust(void);
    uint16_t do_ADC(const uint8_t *waveshape_adc_config_value);

    
    volatile extern uint32_t final_TMR0;
    volatile extern uint8_t prescaler_adjust;
    volatile extern uint32_t raw_TMR0;
    volatile extern uint8_t base_prescaler_bits_index;
    volatile extern uint8_t symmetry_status;
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
    volatile extern uint32_t current_symmetry;
    volatile extern uint8_t current_one_quadrant_index;
    volatile extern uint8_t current_halfcycle;
    volatile extern uint8_t current_quadrant;
    volatile extern uint8_t prescaler_overflow_flag;
    volatile extern uint8_t prescaler_final_index;
#endif


