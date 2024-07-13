#include <xc.h>
#include "wavetables.h"
#include "pinouts.h"
#include "system_uC.h"
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/mcc_generated_files/system/system.h"

void __interrupt(__high_priority) INTERRUPT_InterruptManager(void){
    
    TMR0H = (uint8_t) final_TMR0; //this line must go here, or at least very near the beginning!
    if(TMR0IF == 1){
        INTERRUPT_GlobalInterruptDisable();
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
        else if(current_one_quadrant_index == MIN_QUADRANT_INDEX){
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
            if(current_depth == 255){
                duty = 1023 - duty;
            } 
            else if(current_depth != 0){
                duty = 1023 - ((duty*current_depth) >> 8);
            } 
            else{
                duty = 1023; //if depth is 0, just output 1023
            }
        #endif

        //Write Duty
        set_duty_CCP1(&duty);

        //Finish Up
        LATC5 = 0; //finish ISR length measurement
        INTERRUPT_GlobalInterruptEnable();
    }
}

int main(void)
{
    SYSTEM_Initialize();
    
    get_current_pot_values();
    process_TMR0_raw_speed_and_prescaler();
    process_TMR0_and_prescaler_adjust();
    TMR0H = (uint8_t) final_TMR0;

    INTERRUPT_GlobalInterruptEnable();
    
    while(1){
        
        get_current_pot_values();
        process_TMR0_raw_speed_and_prescaler();
        process_TMR0_and_prescaler_adjust();
        ClrWdt();
    }    
}
