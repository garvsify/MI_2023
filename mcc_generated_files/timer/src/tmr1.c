#include <xc.h>
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/system_uC.h"

/**
 * Section: Global Variables Definitions
*/
volatile uint16_t timer1ReloadVal;
void (*TMR1_InterruptHandler)(void);

const struct TMR_INTERFACE Timer1 = {
    .Initialize = TMR1_Initialize,
    .Start = TMR1_Start,
    .Stop = TMR1_Stop,
    .PeriodCountSet = TMR1_PeriodCountSet,
    .TimeoutCallbackRegister = TMR1_OverflowCallbackRegister,
    .Tasks = NULL
};
static void (*TMR1_OverflowCallback)(void);
static void TMR1_DefaultOverflowCallback(void);
static void (*TMR1_GateCallback)(void);
static void TMR1_DefaultGateCallback(void);

void TMR1_Initialize(void)
{
    //Disable timer
    T1CONbits.TMR1ON = 0;
    //TGGO done; TGSPM disabled; TGTM disabled; TGPOL low; TMRGE disabled; 
    T1GCON = 0x0;
    //TGSS T1GPPS; 
    T1GATE = 0x0;
    //TMRCS LFINTOSC; 
    T1CLK = 0x4;
    //TMRH 255; 
    TMR1H = 0xFF;
    //TMRL 170; 
    TMR1L = 0xAA;

    // Load the TMR1 value to reload variable
    timer1ReloadVal=((uint16_t)TMR1H << 8) | TMR1L;

    //Set default callback for TMR1 overflow interrupt
    TMR1_OverflowCallbackRegister(TMR1_DefaultOverflowCallback);

    //Set default callback for TMR1 gate interrupt
    TMR1_GateCallbackRegister(TMR1_DefaultGateCallback);

    // Clearing TMRI IF flag before enabling the interrupt.
     PIR3bits.TMR1IF = 0;
    // Enabling TMRI interrupt.
     PIE3bits.TMR1IE = 1;
    
    //TMRON enabled; TRD16 disabled; nTSYNC do_not_synchronize; TCKPS 1:8; 
    T1CON = 0x35;
}

void TMR1_Start(void)
{
    // Start the Timer by writing to TMRxON bit
    T1CONbits.TMR1ON = 1;
}

void TMR1_Stop(void)
{
    // Stop the Timer by writing to TMRxON bit
    T1CONbits.TMR1ON = 0;
}

uint16_t TMR1_Read(void)
{
    uint16_t readVal;
    uint8_t readValHigh;
    uint8_t readValLow;
    	
    readValLow = TMR1L;
    readValHigh = TMR1H;
    
    readVal = ((uint16_t)readValHigh << 8) | readValLow;

    return readVal;
}

void TMR1_Write(size_t timerVal)
{
    if (T1CONbits.nT1SYNC == 1)
    {
        // Stop the Timer by writing to TMRxON bit
        T1CONbits.TMR1ON = 0;

        // Write to the Timer1 register
        TMR1H = (uint8_t)(timerVal >> 8);
        TMR1L = (uint8_t)timerVal;

        // Start the Timer after writing to the register
        T1CONbits.TMR1ON = 1;
    }
    else
    {
        // Write to the Timer1 register
        TMR1H = (uint8_t)(timerVal >> 8);
        TMR1L = (uint8_t)timerVal;
    }
}

void TMR1_Reload(void)
{
    TMR1_Write(timer1ReloadVal);
}

void TMR1_PeriodCountSet(size_t periodVal)
{
   timer1ReloadVal = (uint16_t) periodVal;
}

void TMR1_StartSinglePulseAcquisition(void)
{
    T1GCONbits.T1GGO = 1;
}

uint8_t TMR1_CheckGateValueStatus(void)
{
    return (T1GCONbits.T1GVAL);
}

void TMR1_OverflowISR(void)
{

    // Clear the TMR1 interrupt flag
    PIR3bits.TMR1IF = 0;
    TMR1_Write(timer1ReloadVal);

    // ticker function call;
    // ticker is 1 -> Callback function gets called everytime this ISR executes
    if(TMR1_OverflowCallback)
        {
            TMR1_OverflowCallback();
        }
}

void TMR1_OverflowCallbackRegister(void (* CallbackHandler)(void))
{
    TMR1_OverflowCallback = CallbackHandler;
}

static void TMR1_DefaultOverflowCallback(void)
{
    //Add your interrupt code here or
    //Use TMR1_OverflowCallbackRegister function to use Custom ISR
    
    if(*current_adcc_type_ptr == waveshape_adc_config_value){
        
        ADCC_StartConversion(waveshape_adc_config_value); //get depth (12-bit linear)
        
        current_adcc_type_ptr++;
        
    }
    
    else if(*current_adcc_type_ptr == speed_adc_config_value){
        
        ADCC_StartConversion(speed_adc_config_value); //get speed (12-bit linear)
        
        current_adcc_type_ptr++;
        
    }
    
    #if DEPTH_ON_OR_OFF == ON
    
        else if(*current_adcc_type_ptr == depth_adc_config_value){
            
            ADCC_StartConversion(depth_adc_config_value); //get depth (12-bit linear)
            
            current_adcc_type_ptr++;
            
        }
    
    #endif
        
    #if SYMMETRY_ON_OR_OFF == ON

        else if(*current_adcc_type_ptr == symmetry_adc_config_value){
            
            ADCC_StartConversion(symmetry_adc_config_value); //get symmetry (12-bit linear)
            
            current_adcc_type_ptr = adcc_type_array[0];
            
        }
    
    #endif

    TMR1_Stop();
    
    size_t tmr3_value = TMR3_OVERFLOW_COUNT;
    
    TMR3_Write(tmr3_value);
    TMR3_Start(); //time delay to allow ADCC conversion to complete, DMA is triggered on overflow.
    
}

bool TMR1_HasOverflowOccured(void)
{
    return(PIR3bits.TMR1IF);
}

void TMR1_GateISR(void)
{
    // clear the TMR1 interrupt flag
    PIR3bits.TMR1GIF = 0;
    if(TMR1_GateCallback)
    {
        TMR1_GateCallback();
    }
}

void TMR1_GateCallbackRegister(void (* CallbackHandler)(void))
{
    TMR1_GateCallback = CallbackHandler;
}

static void TMR1_DefaultGateCallback(void)
{
    //Add your interrupt code here or
    //Use TMR1_GateCallbackRegister function to use Custom ISR
}


/**
  End of File
*/
