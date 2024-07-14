#include <xc.h>
#include "system_uC.h"

int main(void){
    
    SYSTEM_Initialize();
    
    /*process_TMR0_raw_speed_and_prescaler();
    process_TMR0_and_prescaler_adjust();
    TMR0H = (uint8_t) final_TMR0;
    */
    
    current_waveshape = SQUARE_MODE;
    
    T0CON1bits.CKPS = 0b0100;
    TMR0H = 200;
    TMR0_Start();
    
    INTERRUPT_GlobalInterruptEnable();
    
    while(1){
        
        get_current_pot_values();
        //process_TMR0_raw_speed_and_prescaler();
        //process_TMR0_and_prescaler_adjust();
        
        
        ClrWdt();
    }    
}
