#ifndef CONFIG_H

    #define CONFIG_H

    // PIC18F05Q40 Configuration Bit Settings

    // 'C' source line config statements

    // CONFIG1
    #pragma config FEXTOSC = OFF    // External Oscillator Selection (Oscillator not enabled)
    #pragma config RSTOSC = HFINTOSC_64MHZ// Reset Oscillator Selection (HFINTOSC with HFFRQ = 64 MHz and CDIV = 1:1)

    // CONFIG2
    #pragma config CLKOUTEN = OFF   // Clock out Enable bit (CLKOUT function is disabled)
    #pragma config PR1WAY = OFF     // PRLOCKED One-Way Set Enable bit (PRLOCKED bit can be set and cleared repeatedly)
    #pragma config CSWEN = OFF       // Clock Switch Enable bit (Writing to NOSC and NDIV is not allowed)
    #pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enable bit (Fail-Safe Clock Monitor disabled)
    #pragma config FCMENP = OFF     // Fail-Safe Clock Monitor - Primary XTAL Enable bit (Fail-Safe Clock Monitor disabled)
    #pragma config FCMENS = OFF     // Fail-Safe Clock Monitor - Secondary XTAL Enable bit (Fail-Safe Clock Monitor Disabled)

    // CONFIG3
    #pragma config MCLRE = EXTMCLR  // MCLR Enable bit (If LVP = 0, MCLR pin is MCLR; If LVP = 1, RE3 pin function is MCLR )
    #pragma config PWRTS = PWRT_64  // Power-up timer selection bits (PWRT set at 64ms)
    #pragma config MVECEN = OFF      // Multi-vector enable bit (Multi-vector disabled)
    #pragma config IVT1WAY = OFF    // IVTLOCK bit One-way set enable bit (IVTLOCKED bit can be cleared and set repeatedly)
    #pragma config LPBOREN = ON     // Low Power BOR Enable bit (Low-Power BOR enabled)
    #pragma config BOREN = SBORDIS  // Brown-out Reset Enable bits (Brown-out Reset enabled , SBOREN bit is ignored)

    // CONFIG4
    #pragma config BORV = VBOR_2P85 // Brown-out Reset Voltage Selection bits (Brown-out Reset Voltage (VBOR) set to 2.8V)
    #pragma config ZCD = OFF        // ZCD Disable bit (ZCD module is disabled. ZCD can be enabled by setting the ZCDSEN bit of ZCDCON)
    #pragma config PPS1WAY = OFF    // PPSLOCK bit One-Way Set Enable bit (PPSLOCKED bit can be set and cleared repeatedly (subject to the unlock sequence))
    #pragma config STVREN = ON      // Stack Full/Underflow Reset Enable bit (Stack full/underflow will cause Reset)
    #pragma config LVP = ON         // Low Voltage Programming Enable bit (Low voltage programming enabled. MCLR/VPP pin function is MCLR. MCLRE configuration bit is ignored)
    #pragma config XINST = ON       // Extended Instruction Set Enable bit (Extended Instruction Set and Indexed Addressing Mode enabled)

    // CONFIG5
    #pragma config WDTCPS = WDTCPS_14// WDT Period selection bits (Divider ratio 1:524299)
    #pragma config WDTE = SWDTEN    // WDT operating mode (WDT enabled/disabled by SWDTEN bit)

    // CONFIG6
    #pragma config WDTCWS = WDTCWS_7// WDT Window Select bits (window always open (100%); software control; keyed access not required)
    #pragma config WDTCCS = LFINTOSC// WDT input clock selector (WDT reference clock is the 31.0 kHz LFINTOSC)

    // CONFIG7
    #pragma config BBSIZE = BBSIZE_512// Boot Block Size selection bits (Boot Block size is 512 words)
    #pragma config BBEN = OFF       // Boot Block enable bit (Boot block disabled)
    #pragma config SAFEN = OFF      // Storage Area Flash enable bit (SAF disabled)
    #pragma config DEBUG = ON       // Background Debugger (Background Debugger enabled)

    // CONFIG8
    #pragma config WRTB = OFF       // Boot Block Write Protection bit (Boot Block not Write protected)
    #pragma config WRTC = OFF       // Configuration Register Write Protection bit (Configuration registers not Write protected)
    #pragma config WRTD = OFF       // Data EEPROM Write Protection bit (Data EEPROM not Write protected)
    #pragma config WRTSAF = OFF     // SAF Write protection bit (SAF not Write Protected)
    #pragma config WRTAPP = OFF     // Application Block write protection bit (Application Block not write protected)

    // CONFIG9
    #pragma config CP = OFF         // PFM and Data EEPROM Code Protection bit (PFM and Data EEPROM code protection disabled)

    // #pragma config statements should precede project file includes.
    // Use project enums instead of #define for ON and OFF.

    #include <xc.h>
    #include <stdio.h>
    #include <math.h>

    #define _XTAL_FREQ 64000000

    uint8_t config_int_osc(void);
    uint8_t config_PPS(void);
    uint8_t turn_off_peripherals_not_required(void);
    uint8_t config_ports(void);
    uint8_t config_ADC(void);
    uint16_t do_adc(const uint8_t *modifier);
    uint8_t config_PWM_CCP1(void);
    uint8_t config_TMR0(void);
    uint8_t config_system(void);
    uint8_t turn_on_CCP1_PWM(void);
    uint8_t config_global_interrupts(void);

#endif
    

    
    
// NOTES
    
//TMR2 is used by CCP1 for PWM
//TMR0 is used by program to adjust speed/symmetry