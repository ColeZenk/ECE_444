/**
 * @file main.c
 * @brief Warbling Digital Wire - ECE 444 HW1
 * @author Cole Zenk
 *
 * Implements a digital passthrough with amplitude modulation (warble).
 * Samples ADC at 10kHz, applies envelope, outputs to DAC.
 */

#include "MK22F51212.h"
#include "ADC.h"
#include "DAC.h"
#include "MCG.h"
#include "TimerInt.h"

// Assignment 1 | Warbling Wire
#include "WarblingWire.h"

int main(void) {
    MCG_Clock120_Init();
    ADC_Init();
    ADC_Calibrate();
    DAC_Init();
    TimerInt_Init();

    WarblingWire_Init();
    for(;;) {
        __WFI();  // Sleep until interrupt
    }
}
