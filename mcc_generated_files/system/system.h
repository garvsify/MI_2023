#ifndef SYSTEM_H
#define	SYSTEM_H

#include <xc.h>
#include <stdint.h>
#include <stdbool.h>
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/mcc_generated_files/system/config_bits.h"
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/mcc_generated_files/system/pins.h"
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/mcc_generated_files/adc/adcc.h"
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/mcc_generated_files/pwm/ccp1.h"
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/mcc_generated_files/timer/tmr0.h"
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/mcc_generated_files/timer/tmr2.h"
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/mcc_generated_files/system/interrupt.h"
#include "/Users/jamesgarvey/Documents/Git/MI-2023_2024-PIC18/mcc_generated_files/system/clock.h"


void SYSTEM_Initialize(void);

#endif