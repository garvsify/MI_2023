#ifndef TMR4_H
#define TMR4_H

#include <xc.h>
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/system_uC.h"


/**
 * @ingroup tmr4
 * @brief Defines the Custom Name for the \ref TMR4_Initialize API
 */
#define Timer4_Initialize TMR4_Initialize

/**
 * @ingroup tmr4
 * @brief Defines the Custom Name for the \ref TMR4_ModeSet API
 */
#define Timer4_ModeSet TMR4_ModeSet

/**
 * @ingroup tmr4
 * @brief Defines the Custom Name for the \ref TMR4_ExtResetSourceSet API
 */
#define Timer4_ExtResetSourceSet TMR4_ExtResetSourceSet

/**
 * @ingroup tmr4
 * @brief Defines the Custom Name for the \ref TMR4_Start API
 */
#define Timer4_Start TMR4_Start

/**
 * @ingroup tmr4
 * @brief Defines the Custom Name for the \ref TMR4_Stop API
 */
#define Timer4_Stop TMR4_Stop

/**
 * @ingroup tmr4
 * @brief Defines the Custom Name for the \ref TMR4_Read API
 */
#define Timer4_Read TMR4_Read

/**
 * @ingroup tmr4
 * @brief Defines the Custom Name for the \ref TMR4_Write API
 */
#define Timer4_Write TMR4_Write

/**
 * @ingroup tmr4
 * @brief Defines the Custom Name for the \ref TMR4_PeriodCountSet API
 */
#define Timer4_PeriodCountSet TMR4_PeriodCountSet

/**
 * @ingroup tmr4
 * @brief Defines the Custom Name for the \ref TMR4_OverflowCallbackRegister API
 */
#define Timer4_OverflowCallbackRegister TMR4_OverflowCallbackRegister

/**
 @ingroup tmr4
 @struct TMR_INTERFACE
 @brief This is an instance of TMR_INTERFACE for TMR4 module.
 */
extern const struct TMR_INTERFACE Timer4;

/**
 * @ingroup tmr4
 * @enum TMR4_HLT_EXT_RESET_SOURCE
 * @brief Defines the several modes of operation of the timer with the HLT extension.
 */
typedef enum
{

	/* Roll-over Pulse mode clears the TMRx upon TMRx = PRx, then continue running.
	ON bit must be set and is not affected by Resets
	*/

   /* Roll-over Pulse mode indicates that Timer starts
   immediately upon ON = 1 (Software Control)
   */
   TMR4_ROP_STARTS_TMRON,

   /* Roll-over Pulse mode indicates that the Timer starts
       when ON = 1 and TMRx_ers = 1. Stops when TMRx_ers = 0
     */
   TMR4_ROP_STARTS_TMRON_ERSHIGH,

   /* Roll-over Pulse mode indicates that the Timer starts
      when ON = 1 and TMRx_ers = 0. Stops when TMRx_ers = 1
     */
   TMR4_ROP_STARTS_TMRON_ERSLOW,

   /* Roll-over Pulse mode indicates that the Timer resets
   upon rising or falling edge of TMRx_ers
     */
   TMR4_ROP_RESETS_ERSBOTHEDGE,

   /* Roll-over Pulse mode indicates that the Timer resets
    upon rising edge of TMRx_ers
     */
   TMR4_ROP_RESETS_ERSRISINGEDGE,

   /* Roll-over Pulse mode indicates that the Timer resets
   upon falling edge of TMRx_ers
     */
   TMR4_ROP_RESETS_ERSFALLINGEDGE,

   /* Roll-over Pulse mode indicates that the Timer resets
   upon TMRx_ers = 0
     */
   TMR4_ROP_RESETS_ERSLOW,

   /* Roll-over Pulse mode indicates that the Timer resets
   upon TMRx_ers = 1
     */
   TMR4_ROP_RESETS_ERSHIGH,

    /*In all One-Shot mode the timer resets and the ON bit is
	cleared when the timer value matches the PRx period
	value. The ON bit must be set by software to start
	another timer cycle.
	*/

   /* One shot mode indicates that the Timer starts
    immediately upon ON = 1 (Software Control)
     */
   TMR4_OS_STARTS_TMRON,

   /* One shot mode indicates that the Timer starts
    when a rising edge is detected on the TMRx_ers
     */
   TMR4_OS_STARTS_ERSRISINGEDGE ,

   /* One shot mode indicates that the Timer starts
    when a falling edge is detected on the TMRx_ers
     */
   TMR4_OS_STARTS_ERSFALLINGEDGE ,

   /* One shot mode indicates that the Timer starts
    when either a rising or falling edge is detected on TMRx_ers
     */
   TMR4_OS_STARTS_ERSBOTHEDGE,

   /* One shot mode indicates that the Timer starts
    upon first TMRx_ers rising edge and resets on all
	subsequent TMRx_ers rising edges
     */
   TMR4_OS_STARTS_ERSFIRSTRISINGEDGE,

   /* One shot mode indicates that the Timer starts
    upon first TMRx_ers falling edge and restarts on all
	subsequent TMRx_ers falling edges
     */
   TMR4_OS_STARTS_ERSFIRSTFALLINGEDGE,

   /* One shot mode indicates that the Timer starts
    when a rising edge is detected on the TMRx_ers,
	resets upon TMRx_ers = 0
     */
   TMR4_OS_STARTS_ERSRISINGEDGEDETECT,
     /* One shot mode indicates that the Timer starts
    when a falling edge is detected on the TMRx_ers,
	resets upon TMRx_ers = 1
     */
   TMR4_OS_STARTS_ERSFALLINGEDGEDETECT,
   
   /* One shot mode indicates that the Timer starts
    when a TMRx_ers = 1,ON =1 and resets upon TMRx_ers =0
    */
   TMR4_OS_STARTS_TMRON_ERSHIGH = 0x16,
           
   /* One shot mode indicates that the Timer starts
     when a TMRx_ers = 0,ON = 1 and resets upon TMRx_ers =1 
    */
   TMR4_OS_STARTS_TMRON_ERSLOW = 0x17,
        
   /*In all Mono-Stable mode the ON bit must be initially set,but
     not cleared upon the TMRx = PRx, and the timer will start upon
     an TMRx_ers start event following TMRx = PRx.*/
               
   /* Mono Stable mode indicates that the Timer starts
      when a rising edge is detected on the TMRx_ers and ON = 1
    */
   TMR4_MS_STARTS_TMRON_ERSRISINGEDGEDETECT = 0x11,
           
   /* Mono Stable mode indicates that the Timer starts
      when a falling edge is detected on the TMRx_ers and ON = 1
    */
   TMR4_MS_STARTS_TMRON_ERSFALLINGEDGEDETECT = 0x12,
           
   /* Mono Stable mode indicates that the Timer starts
      when  either a rising or falling edge is detected on TMRx_ers 
      and ON = 1
    */
   TMR4_MS_STARTS_TMRON_ERSBOTHEDGE = 0x13
           
} TMR4_HLT_MODE;

/**
 * @ingroup tmr4
 * @enum TMR4_HLT_EXT_RESET_SOURCE
 * @brief Defines the different reset sources of the HLT.
 */
typedef enum
{
    /*
     * T4INPPS_PIN as the Timer external reset source
     */
    TMR4_T4INPPS_PIN =  0x0,   
    /*
     * TMR2_POSTSCALED as the Timer external reset source
     */
    TMR4_TMR2_POSTSCALED =  0x1,   
    /*
     * CCP1OUT as the Timer external reset source
     */
    TMR4_CCP1OUT =  0x4,   
    /*
     * PWM1_OUT1 as the Timer external reset source
     */
    TMR4_PWM1_OUT1 =  0x5,   
    /*
     * PWM1_OUT2 as the Timer external reset source
     */
    TMR4_PWM1_OUT2 =  0x6,   
    /*
     * PWM2_OUT1 as the Timer external reset source
     */
    TMR4_PWM2_OUT1 =  0x7,   
    /*
     * PWM2_OUT2 as the Timer external reset source
     */
    TMR4_PWM2_OUT2 =  0x8,   
    /*
     * PWM3_OUT1 as the Timer external reset source
     */
    TMR4_PWM3_OUT1 =  0x9,   
    /*
     * PWM3_OUT2 as the Timer external reset source
     */
    TMR4_PWM3_OUT2 =  0xa,   
    /*
     * CMP1_OUT as the Timer external reset source
     */
    TMR4_CMP1_OUT =  0xb,   
    /*
     * CMP2_OUT as the Timer external reset source
     */
    TMR4_CMP2_OUT =  0xc,   
    /*
     * ZCD_OUT as the Timer external reset source
     */
    TMR4_ZCD_OUT =  0xd,   
    /*
     * CLC1_OUT as the Timer external reset source
     */
    TMR4_CLC1_OUT =  0xe,   
    /*
     * CLC2_OUT as the Timer external reset source
     */
    TMR4_CLC2_OUT =  0xf,   
    /*
     * CLC3_OUT as the Timer external reset source
     */
    TMR4_CLC3_OUT =  0x10,   
    /*
     * CLC4_OUT as the Timer external reset source
     */
    TMR4_CLC4_OUT =  0x11,   
    /*
     * UART1_RX_EDGE as the Timer external reset source
     */
    TMR4_UART1_RX_EDGE =  0x12,   
    /*
     * UART1_TX_EDGE as the Timer external reset source
     */
    TMR4_UART1_TX_EDGE =  0x13,   
    /*
     * UART2_RX_EDGE as the Timer external reset source
     */
    TMR4_UART2_RX_EDGE =  0x14,   
    /*
     * UART2_TX_EDGE as the Timer external reset source
     */
    TMR4_UART2_TX_EDGE =  0x15,   
    /*
     * UART3_RX_EDGE as the Timer external reset source
     */
    TMR4_UART3_RX_EDGE =  0x16,   
    /*
     * UART3_TX_EDGE as the Timer external reset source
     */
    TMR4_UART3_TX_EDGE =  0x17   
} TMR4_HLT_EXT_RESET_SOURCE;

/**
  Section: TMR4 APIs
*/

/**
 * @ingroup tmr4
 * @brief Initializes the TMR4 module. This routine must be called before any other timer routines.
 * @param None.
 * @return None.
 */
void TMR4_Initialize(void);

/**
 * @ingroup tmr4
 * @brief Configures the Hardware Limit Timer (HLT) mode.
 * @pre The TMR4 should be initialized with TMR4_Initialize after calling this API.
 * @param mode - Value to write into T4HLTbits.MODE bits.
 * @return None.
 */
void TMR4_ModeSet(TMR4_HLT_MODE mode);

/**
 * @ingroup tmr4
 * @brief Configures the HLT external reset source.
 * @pre The TMR4 should be initialized with TMR4_Initialize after calling this API.
 * @param reset - Value to write into T4RSTbits.RSEL bits.
 * @return None.
 */
void TMR4_ExtResetSourceSet(TMR4_HLT_EXT_RESET_SOURCE reset);

/**
 * @ingroup tmr4
 * @brief Starts TMR4.
 * @pre The TMR4 should be initialized with TMR4_Initialize() before calling this API.
 * @param None.
 * @return None.
 */
void TMR4_Start(void);

/**
 * @ingroup tmr4
 * @brief Stops TMR4.
 * @pre The TMR4 should be initialized with TMR4_Initialize() before calling this API.
 * @param None.
 * @return None.
 */
void TMR4_Stop(void);

/**
 * @ingroup tmr4
 * @brief Reads the 8-bit from the TMR4 register.
 * @pre The TMR4 should be initialized with TMR4_Initialize() before calling this API.
 * @param None.
 * @return 8-bit data from the TMR4 register.
 */
uint8_t TMR4_Read(void);

/**
 * @ingroup tmr4
 * @brief Writes the 8-bit value to the TMR4 register.
 * @pre The TMR4 should be initialized with TMR4_Initialize() before calling this API.
 * @param timerVal - 8-bit value written to the TMR4 register.
 * @return None.
 */
void TMR4_Write(uint8_t timerVal);

/**
 * @ingroup tmr4
 * @brief Loads the 8-bit value to the PR4 register.
 * @pre The TMR4 should be initialized with TMR4_Initialize() before calling this API.
 * @param periodVal - 8-bit value written to the PR4 register.
 * @return None.
 */
void TMR4_PeriodCountSet(size_t periodVal);

/**
 * @ingroup tmr4
 * @brief Interrupt Service Routine (ISR) for TMR4 overflow interrupt.
 * @param None.
 * @return None.
 */
void TMR4_ISR(void);

/**
 * @ingroup tmr4
 * @brief Setter function for the TMR4 overflow callback.
 * @param CallbackHandler - Pointer to the custom callback.
 * @return None.
 */
void TMR4_OverflowCallbackRegister(void (* InterruptHandler)(void));


#endif // TMR4_H
/**
 End of File
*/
