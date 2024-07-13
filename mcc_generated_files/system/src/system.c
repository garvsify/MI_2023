#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/mcc_generated_files/system/system.h"

void SYSTEM_Initialize(void){
    
    CLOCK_Initialize();
    PIN_MANAGER_Initialize();
    ADCC_Initialize();
    PWM1_Initialize();
    Timer0_Initialize();
    TMR2_Initialize();
    INTERRUPT_Initialize();
}



