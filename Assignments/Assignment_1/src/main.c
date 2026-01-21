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

volatile uint16_t adc_sample;

/**
 * @brief PIT Channel 0 ISR - runs at Fs = 10kHz
 */
void PIT0_IRQHandler(void) {
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;

    // TODO: Start ADC conversion and read previous result
    // ADC0->SC1[0] = ADC_SC1_ADCH(channel);
    // adc_sample = ADC0->R[0];

    // TODO: Apply warble envelope
//    uint16_t output = adc_sample;

    // TODO: Write to DAC (12-bit)
    // DAC0->DAT[0].DATL = output & 0xFF;
    // DAC0->DAT[0].DATH = (output >> 8) & 0x0F;
}

int main(void) {
    MCG_Clock120_Init();
    ADC_Init();
    ADC_Calibrate();
    DAC_Init();
    TimerInt_Init();

    while (1) {
        __WFI();  // Sleep until interrupt
    }
}
