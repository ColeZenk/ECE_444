/**
 * @file TimerInt.c
 * @brief PIT (Periodic Interrupt Timer) configuration for K22F
 *
 * Configures PIT Channel 0 for periodic interrupts.
 * Interrupt handler: PIT0_IRQHandler (defined in main.c)
 *
 * Timer formula: LDVAL = (bus_clock / Fs) - 1
 * For 60MHz bus clock and 10kHz: LDVAL = 5999
 */

#include "TimerInt.h"
#include "MK22F51212.h"

/**
 * @brief Initialize PIT Channel 0 for periodic interrupts
 */
void TimerInt_Init(void) {
    SIM->SCGC6 |= SIM_SCGC6_PIT_MASK;
    PIT->MCR = 0;

    PIT->CHANNEL[0].LDVAL = PIT_LDVAL;
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;  // Clear pending
    PIT->CHANNEL[0].TCTRL = PIT_TCTRL_TIE_MASK;  // Enable interrupt

    NVIC_ClearPendingIRQ(PIT0_IRQn);
    NVIC_EnableIRQ(PIT0_IRQn);

    PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;  // Start timer
}
