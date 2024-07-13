#include <xc.h>
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/system_uC.h"

uint8_t SYSTEM_Initialize(void){
    
    CLOCK_Initialize();
    PIN_MANAGER_Initialize();
    ADCC_Initialize();
    PWM1_Initialize();
    Timer0_Initialize();
    TMR2_Initialize();
    INTERRUPT_Initialize();
    
    return 1;
}



