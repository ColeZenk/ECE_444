/**
 * @file DAC.c
 * @brief DAC configuration for K22F
 *
 * Configures DAC0 for 12-bit unbuffered output using VDDA as reference.
 *
 * Output voltage: Vout = VDDA * (value / 4096)
 *
 * Usage:
 *   DAC0->DAT[0].DATL = value & 0xFF;
 *   DAC0->DAT[0].DATH = (value >> 8) & 0x0F;
 */

#include "DAC.h"
#include "MK22F51212.h"

/**
 * @brief Initialize DAC0 for 12-bit output
 *
 * Configuration:
 *   - VDDA as reference voltage
 *   - Software trigger (unbuffered mode)
 *   - High power mode
 */
void DAC_Init(void) {
    SIM->SCGC6 |= SIM_SCGC6_DAC0_MASK;
    DAC0->C0 &= ~DAC_C0_DACEN_MASK;  // Disable during config

    // Zero output
    DAC0->DAT[0].DATL = 0;
    DAC0->DAT[0].DATH = 0;

    // Clear status flags
    DAC0->SR = DAC_SR_DACBFRPTF_MASK;

    // Use VDDA reference, no buffer
    DAC0->C0 = DAC_C0_DACRFS_MASK;
    DAC0->C1 = 0;
    DAC0->C2 = 0;

    DAC0->C0 |= DAC_C0_DACEN_MASK;  // Enable
}
