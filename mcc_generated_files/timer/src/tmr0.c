#include <xc.h>
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/system_uC.h"


const struct TMR_INTERFACE Timer0 = {
    .Initialize = TMR0_Initialize,
    .Start = TMR0_Start,
    .Stop = TMR0_Stop,
    .PeriodCountSet = TMR0_Reload,
    .TimeoutCallbackRegister = TMR0_OverflowCallbackRegister,
    .Tasks = NULL
};

static void (*TMR0_OverflowCallback)(void);
static void TMR0_DefaultOverflowCallback(void);

/**
  Section: TMR0 APIs
*/ 

void TMR0_Initialize(void){

    //TMR0H 255; 
    TMR0H = 0xFF;

    //TMR0L 0; 
    TMR0L = 0x0;

    //T0CS FOSC/4; T0CKPS 1:1; T0ASYNC synchronised; 
    T0CON1 = 0x40;


    //Set default callback for TMR0 overflow interrupt
    TMR0_OverflowCallbackRegister(TMR0_DefaultOverflowCallback);

    //Clear Interrupt flag before enabling the interrupt
    PIR3bits.TMR0IF = 0;
	
    //Enable TMR0 interrupt.
    PIE3bits.TMR0IE = 1;
	
    //T0OUTPS 1:1; T0EN enabled; T016BIT 8-bit; 
    T0CON0 = 0x80;
}

void TMR0_Start(void)
{
    T0CON0bits.T0EN = 1;
}

void TMR0_Stop(void)
{
    T0CON0bits.T0EN = 0;
}

uint8_t TMR0_Read(void)
{
    uint8_t readVal;

    //Read TMR0 register, low byte only
    readVal = TMR0L;

    return readVal;
}

void TMR0_Write(uint8_t timerVal)
{
    //Write to TMR0 register, low byte only
    TMR0L = timerVal;
 }

void TMR0_Reload(size_t periodVal)
{
   //Write to TMR0 register, high byte only
   TMR0H = (uint8_t)periodVal;
}

void TMR0_OverflowISR(void)
{
    //Clear the TMR0 interrupt flag
    PIR3bits.TMR0IF = 0;
    if(TMR0_OverflowCallback)
    {
        TMR0_OverflowCallback();
    }
}

void TMR0_OverflowCallbackRegister(void (* CallbackHandler)(void))
{
    TMR0_OverflowCallback = CallbackHandler;
}

static void TMR0_DefaultOverflowCallback(void)
{
    //TMR0H = (uint8_t)final_TMR0; //this line must go here, or at least very near the beginning!
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
                duty = 1023 - duty*(current_depth >> 8);
            }
            else{
                duty = 1023; //if depth is 0, just output 1023
            }
        #endif

        //Write Duty
        CCP1_LoadDutyValue(duty);

        //Finish Up
        LATC5 = 0; //finish ISR length measurement
        INTERRUPT_GlobalInterruptEnable();
}

