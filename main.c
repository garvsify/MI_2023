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
    TMR4_PeriodCountSet(200);
    TMR4_ChangePrescaler(0b00000111);
    TMR4_Start();
    
    while(1){
        
        //get_current_pot_values();
        //process_TMR0_raw_speed_and_prescaler();
        //process_TMR0_and_prescaler_adjust();
        
        
        ClrWdt();
    }    
}
