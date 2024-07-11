#include "system.h"
#include "config.h"
#include <xc.h>

    volatile uint8_t current_waveshape = 0;
    volatile uint16_t current_speed_linear = 0;
    volatile uint32_t current_speed_linear_32 = 0;
    volatile uint16_t current_depth = 0;
    volatile uint32_t current_symmetry = 0;
    volatile uint8_t current_one_quadrant_index = 0;
    volatile uint8_t current_halfcycle = 0;
    volatile uint8_t current_quadrant = 0;
    volatile uint8_t how_many_128 = 0;
    volatile uint32_t final_TMR0 = 0;
    volatile uint8_t TMR0_prescaler_adjust = 0;
    volatile uint32_t raw_TMR0 = 0;
    volatile uint8_t TMR0_base_prescaler_bits_index = 0;
    volatile uint8_t symmetry_status = 0;
    volatile uint16_t speed_control = 0;
    volatile uint32_t speed_control_32 = 0;
    volatile uint8_t duty_low_byte;
    volatile uint8_t duty_high_byte;
    volatile uint16_t duty = 0;
    volatile uint8_t TMR0_prescaler_overflow_flag = 0;
    volatile uint8_t TMR0_prescaler_final_index = 0;


uint16_t do_ADC(const uint8_t *modifier){
    ADRES = 0x00; //clear first
    ADPCH = *modifier; //select ADC channel
    ADCON0 = ADCON0 | (1 << 7); //turn on ADC
    ADCON0 = ADCON0 | 1; //start A2D
    while(ADCON0bits.GO == 1){} //do nothing while ADC conversion in progress
    ADCON0 = ADCON0 | (0 << 7); //turn off ADC
    uint16_t adc_result = ((uint16_t)(ADRESH << 8) | ADRESL); //concatenate high and low registers to get ADC result
    return adc_result;
}    
    

uint8_t determine_waveshape(void){
    uint16_t ADC = do_ADC(&waveshape_adc_config_value);
    ADC = TENBITMINUSONE - ADC;
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


uint8_t set_duty_CCP1(volatile uint16_t *duty_ptr){
    //we need to split up the duty cycle value (0-1023) into two parts, the 8 MSBs (CCPR3L register) and the 2 LSBs (DC3B bits within the CCP3CON register) and write the separate bit portions to those registers
    CCPR1H = (uint8_t)(*duty_ptr >> 2); //extract 8 MSBs from duty value (dereference the ptr first)
    uint8_t temp = *duty_ptr % 0b100;
    CCPR1L = (uint8_t)(*duty_ptr << 7);
    //although we can write to the above registers at any time to specify the duty cycle, the duty cycle will not update until the
    //timer overflow
    return 1;
}
    

uint8_t get_current_pot_values(void){
    current_waveshape = determine_waveshape();
    current_speed_linear = do_ADC(&speed_adc_config_value); //get speed (10-bit linear)
    current_speed_linear = TENBITMINUSONE - current_speed_linear;
    #if DEPTH_ON_OR_OFF == 1
        current_depth = do_ADC(&depth_adc_config_value); //get depth (10-bit linear)
        current_depth = current_depth >> 2; //convert to 8-bit
        current_depth = EIGHTBITMINUSONE - current_depth;
    #endif
    #if SYMMETRY_ON_OR_OFF == 1
        current_symmetry = do_ADC(&symmetry_adc_config_value); //get symmetry (10-bit linear)
        #if SYMMETRY_ADC_RESOLUTION == 8
            current_symmetry = current_symmetry >> 2; //convert to 8-bit
            current_symmetry = SYMMETRY_ADC_FULL_SCALE - current_symmetry;
        #endif
        #if SYMMETRY_ADC_RESOLUTION == 10
            current_symmetry = current_symmetry;
            current_symmetry = SYMMETRY_ADC_FULL_SCALE - current_symmetry;
        #endif
    #endif
    return 1;
}


uint8_t process_TMR0_raw_speed_and_prescaler(void){
    current_speed_linear_32 = current_speed_linear;
    speed_control_32 = current_speed_linear_32 * NUMBER_OF_FREQUENCY_STEPS;
    speed_control_32 = speed_control_32 >> 10;
    speed_control = (uint16_t) speed_control_32;
    //speed_control = (speed_adc_10_bit/1024)*883
        if(speed_control <= (127-12)){ //inequality is correct!
            raw_TMR0 = (uint8_t) speed_control + 12; //set TMR0
            TMR0_base_prescaler_bits_index = 1;
        }
        else{ 	//(speed_control > (127-12))
            uint16_t speed_control_subtracted;
            speed_control_subtracted = speed_control - (127-12);
            how_many_128 = (uint8_t)(speed_control_subtracted >> 7); //divide by 128, i.e. return how many 128s go into the speed_control
            raw_TMR0 = (uint8_t)(speed_control_subtracted - (uint16_t)(how_many_128 << 7)); //how_many_128*128, set TMR0
            //biggest how_many_128 for NUMBER_OF_FREQUENCY_STEPS == 600 is 3
            //biggest base_prescaler_bits_index == 5 for NUMBER_OF_FREQUENCY_STEPS == 600
            TMR0_base_prescaler_bits_index = (uint8_t)(how_many_128 + 2);   
        }
    return 1;
}


uint8_t adjust_and_set_TMR0_prescaler(void){
    if(TMR0_prescaler_adjust == DIVIDE_BY_TWO){
        TMR0_prescaler_final_index = TMR0_base_prescaler_bits_index + 1;
        T0CON0 = T0CON0 | TMR0_prescaler_bits[TMR0_prescaler_final_index];
    }
    else if(TMR0_prescaler_adjust == DIVIDE_BY_FOUR){
        TMR0_prescaler_final_index = TMR0_base_prescaler_bits_index + 2;
        T0CON0 = T0CON0 | TMR0_prescaler_bits[TMR0_prescaler_final_index];
    }
    else if(TMR0_prescaler_adjust == MULTIPLY_BY_TWO){
        TMR0_prescaler_final_index = TMR0_base_prescaler_bits_index - 1;
        T0CON0 = T0CON0 | TMR0_prescaler_bits[TMR0_prescaler_final_index];
    }
    else if(TMR0_prescaler_adjust == DO_NOTHING){
        TMR0_prescaler_final_index = TMR0_base_prescaler_bits_index;
        T0CON0 = T0CON0 | TMR0_prescaler_bits[TMR0_prescaler_final_index];
    }
    return 1;
}


#if SYMMETRY_ON_OR_OFF == 1
    uint8_t shorten_period(void){
        #if SYMMETRY_ADC_RESOLUTION == 8
            uint32_t twofiftysix_minus_TMR0_final = (((256-raw_TMR0)*(SHORTEN_POWER_OF_TWO_CONSTANT_8_BIT_SYM+(24*current_symmetry)))>>SHORTEN_POWER_OF_TWO_DIVISOR_8_BIT_SYM);
        #endif
        #if SYMMETRY_ADC_RESOLUTION == 10
            uint32_t twofiftysix_minus_TMR0_final = (((256-raw_TMR0)*(SHORTEN_POWER_OF_TWO_CONSTANT_10_BIT_SYM+(24*current_symmetry)))>>SHORTEN_POWER_OF_TWO_DIVISOR_10_BIT_SYM);
        #endif

        final_TMR0 = (256-twofiftysix_minus_TMR0_final);
        TMR0_prescaler_adjust = DO_NOTHING;
        return 1;
    }   

    uint8_t lengthen_period(void){
        #if SYMMETRY_ADC_RESOLUTION == 8
            uint32_t twofiftysix_minus_TMR0_final = (((256-raw_TMR0)*(LENGTHEN_CONSTANT_8_BIT_SYM-(3*current_symmetry)))>>LENGTHEN_POWER_OF_TWO_DIVISOR_8_BIT_SYM);
        #endif
        #if SYMMETRY_ADC_RESOLUTION == 10
            uint32_t twofiftysix_minus_TMR0_final = (((256-raw_TMR0)*(LENGTHEN_CONSTANT_10_BIT_SYM-(3*current_symmetry)))>>LENGTHEN_POWER_OF_TWO_DIVISOR_10_BIT_SYM);
        #endif

        if(twofiftysix_minus_TMR0_final > 256){
            twofiftysix_minus_TMR0_final = (twofiftysix_minus_TMR0_final >> 1);
            final_TMR0 = (256-twofiftysix_minus_TMR0_final);
            TMR0_prescaler_adjust = MULTIPLY_BY_TWO;
        }
        else{
            final_TMR0 = 256-twofiftysix_minus_TMR0_final;
            TMR0_prescaler_adjust = DO_NOTHING;
        }
        return 1;
    }
#endif


uint8_t process_TMR0_and_prescaler_adjust(void){
    #if SYMMETRY_ON_OR_OFF == 1
        if(current_symmetry == SYMMETRY_ADC_HALF_SCALE){
            final_TMR0 = raw_TMR0;
            TMR0_prescaler_adjust = DO_NOTHING;
        }
        else{
            uint8_t symmetry_status = CCW;
            if(current_symmetry > SYMMETRY_ADC_HALF_SCALE){
                current_symmetry = SYMMETRY_ADC_FULL_SCALE - current_symmetry; //converts current_symmetry to 128 -> 0 range (same range as CCW regime, more or less)
                symmetry_status = CW;
            }
            if(current_halfcycle == FIRST_HALFCYCLE){
                if(symmetry_status == CCW){
                    shorten_period();
                }
                else{
                    lengthen_period();
                }
            }
            else if(current_halfcycle == SECOND_HALFCYCLE){
                if(symmetry_status == CW){
                    shorten_period();
                }
                else{
                    lengthen_period();
                }
            }
        }

        adjust_and_set_TMR0_prescaler();

        //Adjust TMR0 for 2 instruction tick delay on update (for low prescaler values)
        if(TMR0_prescaler_final_index == 8){//prescaler is 1:1
            final_TMR0 = final_TMR0 + 2; //(256-TMR0_final) needs to be 2 counts less
        }
        else if(TMR0_prescaler_final_index == 7){//prescaler is 2:1
            final_TMR0 = final_TMR0 + 1; //(256-TMR0_final) needs to be 1 counts less
        }

    #endif

    #if SYMMETRY_ON_OR_OFF == 0
        final_TMR0 = raw_TMR0;
        TMR0_prescaler_adjust = DO_NOTHING;
        adjust_and_set_TMR0_prescaler();
    #endif

    return 1;
}