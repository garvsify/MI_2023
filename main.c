#include <xc.h>
#include <math.h>
#include <stdio.h>
#include "config.h"
#include "system.h"
#include "wavetables.h"

volatile uint16_t duty = 0;
  
void __interrupt() INTERRUPT_InterruptManager(void){
    if(TMR0IF == 1){
    GIE = 0; //disable interrupts
    TMR0 = final_TMR0; //this line must go here!
    //LATC5 = 1; //start ISR length measurement
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
    
    #if DEPTH_ON_OR_OFF == 1
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
    #endif

    //Write Duty
    SET_DUTY_CCP3(&duty);
    
    //Finish Up
    //LATC5 = 0; //finish ISR length measurement
    GIE = 1;
    }
}

void main(void) {
    
    CONFIG_SYSTEM();
    TURN_ON_CCP3_PWM();
    CONFIG_TMR0_INTERRUPT();
    GET_CURRENT_POT_VALUES();
    PROCESS_RAW_SPEED_AND_PRESCALER();

    #if SYMMETRY_ON_OR_OFF == 1
    PROCESS_TMR0_OFFSET_AND_PRESCALER_ADJUST();
    #endif

    PROCESS_FINAL_SPEED_AND_PRESCALER();
    TMR0 = final_TMR0;
    GIE = 1; //enable interrupts
    
    while(1){ //infinite loop
        GET_CURRENT_POT_VALUES();
        PROCESS_RAW_SPEED_AND_PRESCALER();

        #if SYMMETRY_ON_OR_OFF == 1
        PROCESS_TMR0_OFFSET_AND_PRESCALER_ADJUST();
        #endif
        PROCESS_FINAL_SPEED_AND_PRESCALER();
        }
}
       
       
