/**
 * TMR4 Generated Driver File
 *
 * @file tmr4.c
 * 
 * @ingroup  tmr4
 * 
 * @brief API implementations for the TMR4 module.
 *
 * @version TMR4 Driver Version 3.0.4
 */

/*
© [2024] Microchip Technology Inc. and its subsidiaries.

    Subject to your compliance with these terms, you may use Microchip 
    software and any derivatives exclusively with Microchip products. 
    You are responsible for complying with 3rd party license terms  
    applicable to your use of 3rd party software (including open source  
    software) that may accompany Microchip software. SOFTWARE IS ?AS IS.? 
    NO WARRANTIES, WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS 
    SOFTWARE, INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT,  
    MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT 
    WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, 
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY 
    KIND WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF 
    MICROCHIP HAS BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE 
    FORESEEABLE. TO THE FULLEST EXTENT ALLOWED BY LAW, MICROCHIP?S 
    TOTAL LIABILITY ON ALL CLAIMS RELATED TO THE SOFTWARE WILL NOT 
    EXCEED AMOUNT OF FEES, IF ANY, YOU PAID DIRECTLY TO MICROCHIP FOR 
    THIS SOFTWARE.
*/

/**
  Section: Included Files
*/

#include <xc.h>
#include "../tmr4.h"

const struct TMR_INTERFACE Timer4 = {
    .Initialize = TMR4_Initialize,
    .Start = TMR4_Start,
    .Stop = TMR4_Stop,
    .PeriodCountSet = TMR4_PeriodCountSet,
    .TimeoutCallbackRegister = TMR4_OverflowCallbackRegister,
    .Tasks = NULL
};

static void (*TMR4_OverflowCallback)(void);
static void TMR4_DefaultOverflowCallback(void);

/**
  Section: TMR4 APIs
*/

void TMR4_Initialize(void){

    // Set TMR4 to the options selected in the User Interface
    // TCS FOSC/4; 
    T4CLKCON = 0x1;
    // TMODE Software control; TCKSYNC Synchronized; TCKPOL Rising Edge; TPSYNC Not Synchronized; 
    T4HLT = 0x20;
    // TRSEL T4INPPS pin; 
    T4RST = 0x0;
    // PR 255; 
    T4PR = 0xFF;
    // TMR 0x0; 
    T4TMR = 0x0;

    // Set default overflow callback
    TMR4_OverflowCallbackRegister(TMR4_DefaultOverflowCallback);

    // Clearing IF flag before enabling the interrupt.
    PIR10bits.TMR4IF = 0;
    // Enabling TMR4 interrupt.
    PIE10bits.TMR4IE = 1;
    // TCKPS 1:1; TMRON on; TOUTPS 1:1; 
    T4CON = 0x80;
}

void TMR4_ModeSet(TMR4_HLT_MODE mode)
{
   // Configure different types HLT mode
    T4HLTbits.T4MODE = mode;
}

void TMR4_ExtResetSourceSet(TMR4_HLT_EXT_RESET_SOURCE reset)
{
    //Configure different types of HLT external reset source
    T4RSTbits.T4RSEL = reset;
}

void TMR4_Start(void)
{
    // Start the Timer by writing to TMRxON bit
    T4CONbits.TMR4ON = 1;
}

void TMR4_Stop(void)
{
    // Stop the Timer by writing to TMRxON bit
    T4CONbits.TMR4ON = 0;
}

uint8_t TMR4_Read(void)
{
    uint8_t readVal;
    readVal = TMR4;
    return readVal;
}

void TMR4_Write(uint8_t timerVal)
{
    // Write to the Timer4 register
    TMR4 = timerVal;;
}

void TMR4_PeriodCountSet(size_t periodVal)
{
   PR4 = (uint8_t) periodVal;
}

void TMR4_ISR(void)
{
    // clear the TMR4 interrupt flag
     PIR10bits.TMR4IF = 0;

    if(TMR4_OverflowCallback)
    {
        TMR4_OverflowCallback();
    }
}

void TMR4_OverflowCallbackRegister(void (* InterruptHandler)(void))
{
   TMR4_OverflowCallback = InterruptHandler;
}

static void TMR4_DefaultOverflowCallback(void)
{
    //TMR0H = (uint8_t) final_TMR0; //this line must go here, or at least very near the beginning!
    TMR0H = 255;
    if(TMR4IF == 1){
        
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
        CCP1_LoadDutyValue(duty);

        //Finish Up
        LATC5 = 0; //finish ISR length measurement
        INTERRUPT_GlobalInterruptEnable();
    }
}

