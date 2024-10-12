#include <xc.h>
#include "system_uC.h"


/*IMPORTANT INFO ABOUT BUILDING:

Use XC8 v2.50 and Pack v1.25.433

*/

/*IMPORTANT INFO ABOUT USAGE:
 
Usage of each peripheral:

 - TMR0 generates the interrupt to change the value loaded into the TMR0H register, which controls the frequency of the oscillation.
 - TMR1 is currently unused but could be used for DMA to generate the interrupt to trigger the data transfer of ADC values into working variables.
 - TMR2 is used exclusively by the CCP module and interrupts should not be enabled
 
*/

int main(void){
    
    SYSTEM_Initialize();
    
    /*process_TMR0_raw_speed_and_prescaler();
    process_TMR0_and_prescaler_adjust();
    TMR0H = (uint8_t) final_TMR0;
    */
    
    T0CON1bits.CKPS = 0b0100;
    TMR0H = 200;
    final_TMR0 = 200;
    TMR0_Start();
    
    INTERRUPT_GlobalInterruptEnable();
    
    while(1){
        
        /*get_current_pot_values();
        process_TMR0_raw_speed_and_prescaler();
        process_TMR0_and_prescaler_adjust();*/
        
        
        ClrWdt();
    }    
}
