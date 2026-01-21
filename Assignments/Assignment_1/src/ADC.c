/**
 * @file ADC.c
 * @brief ADC configuration and calibration for K22F
 *
 * Configures ADC0 for 12-bit single-ended conversions.
 * Uses bus clock / 4 as ADC clock source.
 */

#include "ADC.h"
#include "MK22F51212.h"

/**
 * @brief Initialize ADC0 for 12-bit single-ended conversion
 *
 * Configuration:
 *   - 12-bit resolution
 *   - Bus clock / 4 as clock source
 *   - Software trigger
 *   - VREFH/VREFL as reference
 */
void ADC_Init(void) {
    SIM->SCGC6 |= SIM_SCGC6_ADC0_MASK;
    ADC0->SC1[0] = ADC_SC1_ADCH(0x1F);  // Disable module during config

    // 12-bit mode, bus clock / 4
    ADC0->CFG1 = ADC_CFG1_ADIV(2) |
                 ADC_CFG1_MODE(1) |
                 ADC_CFG1_ADICLK(0);

    ADC0->CFG2 = ADC_CFG2_ADLSTS(3);

    // Clear compare registers
    ADC0->CV1 = 0;
    ADC0->CV2 = 0;

    // Software trigger, external reference
    ADC0->SC2 = ADC_SC2_REFSEL(0);

    // No averaging, single conversion
    ADC0->SC3 = 0;
}

/**
 * @brief Run ADC self-calibration sequence
 *
 * Must be called after ADC_Init(). Configures ADC for optimal
 * calibration (slow clock, 32-sample averaging), runs calibration,
 * computes gain correction values, then restores normal config.
 *
 * @note Blocks until calibration complete. Hangs if calibration fails.
 */
void ADC_Calibrate(void) {
    uint16_t cal_var;

    // Slow clock + hardware averaging for calibration accuracy
    ADC0->CFG1 |= ADC_CFG1_ADIV(3);
    ADC0->SC3 = ADC_SC3_AVGE_MASK |
                ADC_SC3_AVGS(3) |
                ADC_SC3_CAL_MASK;

    while (!(ADC0->SC1[0] & ADC_SC1_COCO_MASK));
    while (ADC0->SC3 & ADC_SC3_CALF_MASK);  // Hang if cal failed

    // Plus-side gain calibration
    cal_var = ADC0->CLP0 + ADC0->CLP1 + ADC0->CLP2 +
              ADC0->CLP3 + ADC0->CLP4 + ADC0->CLPS;
    cal_var = (cal_var >> 1) | 0x8000;
    ADC0->PG = cal_var;

    // Minus-side gain calibration
    cal_var = ADC0->CLM0 + ADC0->CLM1 + ADC0->CLM2 +
              ADC0->CLM3 + ADC0->CLM4 + ADC0->CLMS;
    cal_var = (cal_var >> 1) | 0x8000;
    ADC0->MG = cal_var;

    // Restore normal config
    ADC_Init();
}
