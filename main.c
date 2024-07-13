#include <xc.h>
#include "wavetables.h"
#include "pinouts.h"
#include "system_uC.h"
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/mcc_generated_files/system/system.h"

int main(void)
{
    SYSTEM_Initialize();
    
    get_current_pot_values();
    process_TMR0_raw_speed_and_prescaler();
    process_TMR0_and_prescaler_adjust();
    TMR0H = (uint8_t) final_TMR0;

    //INTERRUPT_GlobalInterruptEnable();
    
    while(1){
        
        get_current_pot_values();
        process_TMR0_raw_speed_and_prescaler();
        process_TMR0_and_prescaler_adjust();
        ClrWdt();
    }    
}
