#include "system.h"

    volatile uint8_t current_waveshape = 0;
    volatile uint16_t current_speed_linear = 0;
    volatile uint32_t current_speed_linear_32 = 0;
    volatile uint16_t current_depth = 0;
    volatile uint24_t current_symmetry = 0;
    volatile uint8_t current_one_quadrant_index = 0;
    volatile uint8_t current_halfcycle = 0;
    volatile uint8_t current_quadrant = 0;
    volatile uint8_t how_many_128 = 0;
    volatile uint24_t final_TMR0 = 0;
    volatile uint8_t prescaler_adjust = 0;
    volatile uint24_t raw_TMR0 = 0;
    volatile uint8_t base_prescaler_bits_index = 0;
    volatile uint8_t symmetry_status = 0;
    volatile uint16_t speed_control = 0;
    volatile uint32_t speed_control_32 = 0;
    volatile uint8_t duty_low_byte;
    volatile uint8_t duty_high_byte;
    volatile uint16_t duty = 0;
    volatile uint8_t prescaler_overflow_flag = 0;
    volatile uint8_t prescaler_final_index = 0;

    
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
        #if SYMMETRY_ADC_RESOLUTION == 8
            current_symmetry = current_symmetry >> 2; //convert to 8-bit
        #endif
        #if SYMMETRY_ADC_RESOLUTION == 10
            current_symmetry = current_symmetry;
        #endif
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
            raw_TMR0 = (uint8_t)(speed_control_subtracted - (uint16_t)(how_many_128 << 7)); //how_many_128*128, set TMR0
            //biggest how_many_128 for NUMBER_OF_FREQUENCY_STEPS == 600 is 3
            //biggest base_prescaler_bits_index == 5 for NUMBER_OF_FREQUENCY_STEPS == 600
            base_prescaler_bits_index = (uint8_t)(how_many_128 + 2);   
        }
    return 1;
}


uint8_t CHECK_IF_PRESCALER_NEEDS_TO_BE_1_1(void){
    if((base_prescaler_bits_index + 1) > 7){
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


uint8_t ADJUST_AND_SET_PRESCALER(void){
    if(prescaler_adjust == DIVIDE_BY_TWO){
        prescaler_overflow_flag = CHECK_IF_PRESCALER_NEEDS_TO_BE_1_1();
            if(prescaler_overflow_flag){
                TURN_PRESCALER_OFF();
                return 1;
            }
            else{
                TURN_PRESCALER_ON();
                OPTION_REG = prescaler_bits[base_prescaler_bits_index + 1];
                prescaler_final_index = base_prescaler_bits_index + 1;
            }
    }
    else if(prescaler_adjust == DIVIDE_BY_FOUR){
        prescaler_overflow_flag = CHECK_IF_PRESCALER_NEEDS_TO_BE_1_1();
            if(prescaler_overflow_flag){
                TURN_PRESCALER_OFF();
                return 1;
            }
            else{
                TURN_PRESCALER_ON();
                OPTION_REG = prescaler_bits[base_prescaler_bits_index + 2];
                prescaler_final_index = base_prescaler_bits_index + 1;
            }
    }
    else if(prescaler_adjust == MULTIPLY_BY_TWO){
        OPTION_REG = prescaler_bits[base_prescaler_bits_index - 1];
        prescaler_final_index = base_prescaler_bits_index - 1;
    }
    else if(prescaler_adjust == DO_NOTHING){
        OPTION_REG = prescaler_bits[base_prescaler_bits_index];
        prescaler_final_index = base_prescaler_bits_index;
    }
    return 1;
}

#if SYMMETRY_ON_OR_OFF == 1
    uint8_t SHORTEN_PERIOD(void){
        #if SYMMETRY_ADC_RESOLUTION == 8
            uint24_t twofiftysix_minus_TMR0_final = (((256-raw_TMR0) * (SHORTEN_POWER_OF_TWO_CONSTANT_8_BIT_SYM+(24*current_symmetry))) >> SHORTEN_POWER_OF_TWO_DIVISOR_8_BIT_SYM);
        #endif

        final_TMR0 = (256 - twofiftysix_minus_TMR0_final);
        prescaler_adjust = DO_NOTHING;
        return 1;

    }   

    uint8_t LENGTHEN_PERIOD(void){
        #if SYMMETRY_ADC_RESOLUTION == 8
            uint24_t twofiftysix_minus_TMR0_final = (((256-raw_TMR0) * (LENGTHEN_CONSTANT_8_BIT_SYM-(3*current_symmetry))) >> LENGTHEN_POWER_OF_TWO_DIVISOR_8_BIT_SYM);
        #endif

        if(twofiftysix_minus_TMR0_final > 256){
            twofiftysix_minus_TMR0_final = (twofiftysix_minus_TMR0_final >> 1);
            final_TMR0 = (256 - twofiftysix_minus_TMR0_final);
            prescaler_adjust = MULTIPLY_BY_TWO;
        }
        else{
            final_TMR0 = 256 - twofiftysix_minus_TMR0_final;
            prescaler_adjust = DO_NOTHING;
        }
        return 1;
    }
#endif


uint8_t PROCESS_TMR0_AND_PRESCALER_ADJUST(void){
    #if SYMMETRY_ON_OR_OFF == 1
        if(current_symmetry == SYMMETRY_ADC_HALF_SCALE){
            final_TMR0 = raw_TMR0;
            prescaler_adjust = DO_NOTHING;
        }
        else{
        uint8_t symmetry_status = CCW;
        if(current_symmetry > SYMMETRY_ADC_HALF_SCALE){
            current_symmetry = SYMMETRY_ADC_FULL_SCALE - current_symmetry; //converts current_symmetry to 128 -> 0 range (same range as CCW regime, more or less)
            symmetry_status = CW;
        }

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
        }

        ADJUST_AND_SET_PRESCALER();

        //Adjust TMR0 for 2 instruction tick delay on update (for low prescaler values)
        if(prescaler_overflow_flag == 1){//prescaler is 1:1
            final_TMR0 = final_TMR0 + 2; //(256-TMR0_final) needs to be 2 counts less
        }
        else if(prescaler_final_index == 7){//prescaler is 2:1
            final_TMR0 = final_TMR0 + 1; //(256-TMR0_final) needs to be 1 counts less
        }

    #endif

    #if SYMMETRY_ON_OR_OFF == 0
        final_TMR0 = raw_TMR0;
        prescaler_adjust = DO_NOTHING;
    #endif

    return 1;
}