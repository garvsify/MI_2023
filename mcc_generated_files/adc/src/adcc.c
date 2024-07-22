#include <xc.h>
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/system_uC.h"

static void (*ADCC_ADI_InterruptHandler)(void);
static void ADCC_DefaultADI_ISR(void);

/**
  Section: ADCC Module APIs
*/

void ADCC_Initialize(void){
    
    // set the ADCC to the options selected in the User Interface
    //ADLTHL 0; 
    ADLTHL = 0x0;
    //ADLTHH 0; 
    ADLTHH = 0x0;
    //ADUTHL 0; 
    ADUTHL = 0x0;
    //ADUTHH 0; 
    ADUTHH = 0x0;
    //ADSTPTL 0; 
    ADSTPTL = 0x0;
    //ADSTPTH 0; 
    ADSTPTH = 0x0;
    //ADACCU 0x0; 
    ADACCU = 0x0;
    //ADRPT 0; 
    ADRPT = 0x0;
    //ADCHS ANC0; 
    ADPCH = 0x10;
    //ADACQL 82; //increasing aquisition time to ~2us
    ADACQL = 0x82;
    //ADACQH 0; 
    ADACQH = 0x0;
    //ADCAP Additional uC disabled; 
    ADCAP = 0x0;
    //ADPREL 15; 
    ADPREL = 0xF;
    //ADPREH 0; 
    ADPREH = 0x0;
    //ADDSEN disabled; ADGPOL digital_low; ADIPEN disabled; ADPPOL Vdd; 
    ADCON1 = 0x80;
    //ADCRS 1; ADMD Basic_mode; ADACLR disabled; ADPSIS RES; 
    ADCON2 = 0x10;
    //ADCALC First derivative of Single measurement; ADTMD disabled; ADSOI ADGO not cleared; 
    ADCON3 = 0x0;
    //ADMATH registers not updated; 
    ADSTAT = 0x0;
    //ADNREF VSS; ADPREF VDD; 
    ADREF = 0x0;
    //ADACT disabled; 
    ADACT = 0x0;
    //ADCCS FOSC/64; 
    ADCLK = 0x1F;
    //GO_nDONE undefined; ADFM right; ADON enabled; ADCS FOSC; ADCONT disabled; 
    ADCON0 = 0x84;
    
    ADCPbits.CPON = 1; //turn on charge pump, will this fix things?
    ADCON0bits.ADCONT = 0; //fix things plz?
    
    //Clear the ADC interrupt flag
    PIR1bits.ADIF = 0;
    
    //Configure interrupt handlers
    ADCC_SetADIInterruptHandler(&ADCC_DefaultADI_ISR);
    
    //Clear the ADC Threshold interrupt flag
    PIR2bits.ADTIF = 0;
    
    // Enabling ADCC interrupt.
    PIE1bits.ADIE = 1;
}

void ADCC_StartConversion(adcc_channel_t channel)
{
    
    while(!ADCC_IsConversionDone()); // make sure no other conversion occurring.
    
    //Selects the A/D channel
    ADPCH = channel;

    //Starts the conversion
    ADCON0bits.ADGO = 1;
}

bool ADCC_IsConversionDone(void)
{
    //Starts the conversion
    return ((unsigned char)(!ADCON0bits.ADGO));
}

adc_result_t ADCC_GetConversionResult(void)
{
    
    //Returns the result
    return ((adc_result_t)((ADRESH << 8) | ADRESL));
}

adc_result_t ADCC_GetSingleConversion(adcc_channel_t channel)
{
    //Selects the A/D channel
    ADPCH = channel;  
    
    //Disables the continuous mode
    ADCON0bits.ADCONT = 0;

    //Starts the conversion
    ADCON0bits.ADGO = 1;


    //Waits for the conversion to finish
    while (ADCON0bits.ADGO)
    {
    }
    
    //Conversion finished, returns the result
    return ((adc_result_t)((ADRESH << 8) | ADRESL));
}

inline void ADCC_StopConversion(void)
{
    //Resets the ADGO bit.
    ADCON0bits.ADGO = 0;
}

inline void ADCC_SetStopOnInterrupt(void)
{
    //Sets the ADSOI bit.
    ADCON3bits.ADSOI = 1;
}

inline void ADCC_DischargeSampleCapacitor(void)
{
    //Sets the ADC channel to AVss.
    ADPCH = 0x3B;   
}

void ADCC_LoadAcquisitionRegister(uint16_t acquisitionValue)
{
    //Loads the ADACQH and ADACQL registers.
    ADACQH = (uint8_t) (acquisitionValue >> 8);
    ADACQL = (uint8_t) acquisitionValue;  
}

void ADCC_SetPrechargeTime(uint16_t prechargeTime)
{
    //Loads the ADPREH and ADPREL registers.
    ADPREH = (uint8_t) (prechargeTime >> 8);
    ADPREL = (uint8_t) prechargeTime;
}

void ADCC_SetRepeatCount(uint8_t repeatCount)
{
    //Loads the ADRPT register.
    ADRPT = repeatCount;   
}

uint8_t ADCC_GetCurrentCountofConversions(void)
{
    //Returns the contents of ADCNT register
    return ADCNT;
}

inline void ADCC_ClearAccumulator(void)
{
    //Resets the ADCON2bits.ADACLR bit.
    ADCON2bits.ADACLR = 1;
}

uint24_t ADCC_GetAccumulatorValue(void)
{
    //Returns the contents of ADACCU, ADACCH and ADACCL registers
    return (((uint24_t)ADACCU << 16)+((uint24_t)ADACCH << 8) + ADACCL);
}

bool ADCC_HasAccumulatorOverflowed(void)
{
    //Returns the status of ADSTATbits.ADAOV
    return ADSTATbits.ADAOV;
}

uint16_t ADCC_GetFilterValue(void)
{
    //Returns the contents of ADFLTRH and ADFLTRL registers
    return ((uint16_t)((ADFLTRH << 8) + ADFLTRL));
}

uint16_t ADCC_GetPreviousResult(void)
{
    //Returns the contents of ADPREVH and ADPREVL registers
    return ((uint16_t)((ADPREVH << 8) + ADPREVL));
}

void ADCC_DefineSetPoint(uint16_t setPoint)
{
    //Sets the ADSTPTH and ADSTPTL registers
    ADSTPTH = (uint8_t) (setPoint >> 8);
    ADSTPTL = (uint8_t) setPoint;
}

void ADCC_SetUpperThreshold(uint16_t upperThreshold)
{
    //Sets the ADUTHH and ADUTHL registers
    ADUTHH = (uint8_t) (upperThreshold >> 8);
    ADUTHL = (uint8_t) upperThreshold;
}

void ADCC_SetLowerThreshold(uint16_t lowerThreshold)
{
    //Sets the ADLTHH and ADLTHL registers
    ADLTHH = (uint8_t) (lowerThreshold >> 8);
    ADLTHL = (uint8_t) lowerThreshold;
}

uint16_t ADCC_GetErrorCalculation(void)
{
    //Returns the contents of ADERRH and ADERRL registers
    return ((uint16_t)((ADERRH << 8) + ADERRL));
}

inline void ADCC_EnableDoubleSampling(void)
{
    //Sets the ADCON1bits.ADDSEN
    ADCON1bits.ADDSEN = 1;
}

inline void ADCC_EnableContinuousConversion(void)
{
    //Sets the ADCON0bits.ADCONT
    ADCON0bits.ADCONT = 1;
}

inline void ADCC_DisableContinuousConversion(void)
{
    //Resets the ADCON0bits.ADCONT
    ADCON0bits.ADCONT = 0;
}

bool ADCC_HasErrorCrossedUpperThreshold(void)
{
    //Returns the value of ADSTATbits.ADUTHR bit.
    return ADSTATbits.ADUTHR;
}

bool ADCC_HasErrorCrossedLowerThreshold(void)
{
    //Returns the value of ADSTATbits.ADLTHR bit.
    return ADSTATbits.ADLTHR;
}

uint8_t ADCC_GetConversionStageStatus(void)
{
    //Returns the contents of ADSTATbits.ADSTAT field.
    return ADSTATbits.ADSTAT;
}

void ADCC_ISR(void)
{
    //Clears the ADCC interrupt flag
    PIR1bits.ADIF = 0;

    if (ADCC_ADI_InterruptHandler != NULL)
    {
        ADCC_ADI_InterruptHandler();
    }
}

void ADCC_SetADIInterruptHandler(void (* InterruptHandler)(void))
{
    ADCC_ADI_InterruptHandler = InterruptHandler;
}

static void ADCC_DefaultADI_ISR(void){
    
    ADC_result = ADCC_GetConversionResult();
        
    if(ADC_type_flag == WAVESHAPE_FLAG){
        
        ADC_result = TWELVEBITMINUSONE - ADC_result;
        
        if(ADC_result <= TRIANGLE_MODE_ADC_THRESHOLD){
            current_waveshape = TRIANGLE_MODE; //triangle wave
        }
        else if (ADC_result <= SINE_MODE_ADC_THRESHOLD){
            current_waveshape = SINE_MODE; //sine wave
        }
        else if (ADC_result <= SQUARE_MODE_ADC_THRESHOLD){
            current_waveshape = SQUARE_MODE; //square wave
        }
        else{
            current_waveshape = SINE_MODE; //if error, return sine
        }
    }
    
    else if(ADC_type_flag == SPEED_FLAG){
        
        current_speed_linear = ADC_result;
        current_speed_linear = TWELVEBITMINUSONE - current_speed_linear;
    }
    
    else if(ADC_type_flag == DEPTH_FLAG){
        
        current_depth = ADC_result;
        current_depth = current_depth >> 2; //convert to 8-bit
        current_depth = EIGHTBITMINUSONE - current_depth;
    }
    
    else if(ADC_type_flag == SYMMETRY_FLAG){
        
        current_symmetry = ADC_result;
        
        #if SYMMETRY_ADC_RESOLUTION == 8
            current_symmetry = current_symmetry >> 2; //convert to 8-bit
        #endif

        current_symmetry = SYMMETRY_ADC_FULL_SCALE - current_symmetry;
    }
}

