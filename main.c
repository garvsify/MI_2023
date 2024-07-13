#include <xc.h>
#include "system_uC.h"

int main(void)
{
    SYSTEM_Initialize();
    
    /*get_current_pot_values();
    process_TMR0_raw_speed_and_prescaler();
    process_TMR0_and_prescaler_adjust();
    TMR0H = (uint8_t) final_TMR0;
    */
    
    INTERRUPT_GlobalInterruptEnable();
    current_waveshape = SINE_MODE;
    TMR0_Reload(200);
    TMR0_Start();
    
    while(1){
        
        T0CON1 = T0CON1 | 0b00001000;
        
        //get_current_pot_values();
        //process_TMR0_raw_speed_and_prescaler();
        //process_TMR0_and_prescaler_adjust();
        
        
        ClrWdt();
    }    
}
