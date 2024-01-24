#include <xc.h>
#include "system.h"

    volatile uint8_t current_waveshape = 0;
    volatile uint16_t current_speed_linear = 0;
    volatile uint32_t current_speed_linear_32 = 0;
    volatile uint16_t current_depth = 0;
    volatile uint16_t current_symmetry = 0;
    volatile uint8_t current_one_quadrant_index = 0;
    volatile uint8_t current_halfcycle = 0;
    volatile uint8_t current_quadrant = 0;
    volatile uint8_t how_many_128 = 0;
    volatile uint8_t final_TMR0 = 0;
    volatile uint8_t TMR0_offset = 0;
    volatile uint8_t TMR0_offset_sign = 0;
    volatile uint8_t prescaler_adjust = 0;
    volatile uint8_t raw_TMR0 = 0;
    volatile uint8_t base_prescaler_bits_index = 0;
    volatile uint16_t dTMR0_ideal = 0;
    volatile uint8_t clear_TMR0_please = 0;
    volatile uint8_t symmetry_status = 0;
    volatile uint32_t symmetry_total = 0;
    volatile uint16_t speed_control = 0;
    volatile uint32_t speed_control_32 = 0;;

    
uint8_t DETERMINE_WAVESHAPE(void){
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
    

uint8_t GET_CURRENT_POT_VALUES(void){
    current_waveshape = DETERMINE_WAVESHAPE();
    current_speed_linear = DO_ADC(&speed_adc_config_value); //get speed (10-bit linear)
    #if DEPTH_ON_OR_OFF == 1
        current_depth = DO_ADC(&depth_adc_config_value); //get depth (10-bit linear)
        current_depth = current_depth >> 2; //convert to 8-bit
    #endif
    #if SYMMETRY_ON_OR_OFF == 1
        current_symmetry = DO_ADC(&symmetry_adc_config_value); //get symmetry (10-bit linear)
        uint8_t symmetry_ADC_resolution = SYMMETRY_ADC_RESOLUTION;
        if(symmetry_ADC_resolution == 8){
            current_symmetry = current_symmetry >> 2; //convert to 8-bit
        }
    #endif
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
            raw_TMR0 = (uint8_t) (speed_control_subtracted - (uint16_t)(how_many_128 << 7)); //how_many_128*128, set TMR0
            //biggest how_many_128 for NUMBER_OF_FREQUENCY_STEPS == 600 is 3
            //biggest base_prescaler_bits_index == 5 for NUMBER_OF_FREQUENCY_STEPS == 600
            base_prescaler_bits_index = (uint8_t)(how_many_128 + 2);
            /*
            uint8_t prescaler_overflow_flag = CHECK_IF_PRESCALER_NEEDS_TO_BE_1_1();
            if(prescaler_overflow_flag){
                TURN_PRESCALER_OFF();
            }
            else{
                TURN_PRESCALER_ON();
            */
        }
    return 1;
}


uint8_t CHECK_IF_PRESCALER_NEEDS_TO_BE_1_1(void){
    if(((base_prescaler_bits_index + 1) > 7)){
        //set prescaler to 1:1
        return 1;
    }
    else{
        return 0;
    }
}


uint8_t TURN_PRESCALER_OFF(void){
    OPTION_REG = OPTION_REG & (1<<3); //turn off prescaler to select 1:1
    return 1;
}


uint8_t TURN_PRESCALER_ON(void){
    OPTION_REG = OPTION_REG & (0<<3); //turn on prescaler
    return 1;
}


uint8_t PROCESS_FINAL_SPEED_AND_PRESCALER(void){
    #if SYMMETRY_ON_OR_OFF == 1

    ADJUST_TMR0();

   uint8_t prescaler_overflow_flag = CHECK_IF_PRESCALER_NEEDS_TO_BE_1_1();


    if(prescaler_overflow_flag){
        TURN_PRESCALER_OFF();
        return 1;
    }
    else{
        TURN_PRESCALER_ON();
    }

    ADJUST_PRESCALER();
    #endif

    
    #if SYMMETRY_ON_OR_OFF == 0
        final_TMR0 = raw_TMR0;
        OPTION_REG = prescaler_bits[base_prescaler_bits_index];
    #endif

    return 1;
}   


#if SYMMETRY_ON_OR_OFF == 1
uint8_t ADJUST_TMR0(void){
    if(clear_TMR0_please){
        raw_TMR0 = 0;
    }
    if((TMR0_offset_sign == POSITIVE) || (TMR0_offset_sign == DONT_CARE)){
        final_TMR0 = raw_TMR0 + TMR0_offset;
    }
    else if(TMR0_offset_sign == NEGATIVE){
        final_TMR0 = raw_TMR0 - TMR0_offset;
    }
    return 1;
}

uint8_t ADJUST_PRESCALER(void){
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
    uint16_t dTMR0_ideal = (uint16_t)((uint32_t)(symmetry_total * SHORTEN_PERIOD_FRACTION_16_BIT_NUMBER) >> RESOLUTION_OF_TOTAL_SYMMETRY_FRACTION);

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
    uint16_t dTMR0_ideal = (uint16_t)((uint32_t)(symmetry_total * LENGTHEN_PERIOD_FRACTION_16_BIT_NUMBER) >> RESOLUTION_OF_TOTAL_SYMMETRY_FRACTION);
    
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
    if(current_symmetry == SYMMETRY_ADC_HALF_SCALE){
        TMR0_offset = 0;
        TMR0_offset_sign = POSITIVE;
        prescaler_adjust = DO_NOTHING;
        clear_TMR0_please = NO;
        return 1; //exit function
    }
    uint8_t symmetry_status = CCW;
    if(current_symmetry > SYMMETRY_ADC_HALF_SCALE){
        current_symmetry = SYMMETRY_ADC_FULL_SCALE - current_symmetry; //converts current_symmetry to 128 -> 0 range (same range as CCW regime, more or less)
        symmetry_status = CW;
    }
    
    uint32_t temp = (uint32_t)(MAX_SYMMETRY_TOTAL * (uint32_t)(SYMMETRY_ADC_HALF_SCALE - current_symmetry)); //must be 32-bit int if ADC is ever 10-bit
    symmetry_total = (temp >> SYMMETRY_ADC_HALF_SCALE_NO_BITS);

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
#endif