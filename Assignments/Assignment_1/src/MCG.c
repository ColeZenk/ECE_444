/**
 * @file MCG.c
 * @brief Multipurpose Clock Generator configuration for K22F
 *
 * Configures system for 120MHz core clock from 8MHz external crystal.
 *
 * Clock outputs:
 *   - Core:    120MHz
 *   - Bus:     60MHz  (PIT, ADC, etc.)
 *   - FlexBus: 30MHz
 *   - Flash:   24MHz
 */

#include "MCG.h"
#include "MK22F51212.h"

/**
 * @brief Initialize MCG for 120MHz operation
 *
 * Transitions through FEI -> FBE -> PBE -> PEE modes to achieve
 * 120MHz core clock from 8MHz external crystal.
 *
 * PLL config: 8MHz / 2 * 30 = 120MHz
 */
void MCG_Clock120_Init(void) {

    // Configure external crystal pins (PTA18/PTA19)
    SIM->SCGC5 |= SIM_SCGC5_PORTA_MASK;
    PORTA->PCR[18] = PORT_PCR_MUX(0);
    PORTA->PCR[19] = PORT_PCR_MUX(0);

    // Enable High Speed Run mode (required for >80MHz)
    SMC->PMPROT = SMC_PMPROT_AHSRUN_MASK;
    SMC->PMCTRL = SMC_PMCTRL_RUNM(3u);

    // Clock dividers: Core=120MHz, Bus=60MHz, FlexBus=30MHz, Flash=24MHz
    SIM->CLKDIV1 = SIM_CLKDIV1_OUTDIV2(1u) |
                   SIM_CLKDIV1_OUTDIV3(3u) |
                   SIM_CLKDIV1_OUTDIV4(4u);

    // FEI -> FBE: Switch to external 8MHz crystal
    MCG->C2 = MCG_C2_RANGE(1u) |
              MCG_C2_EREFS_MASK;
    MCG->C1 = MCG_C1_CLKS(2u) |
              MCG_C1_FRDIV(3u);

    while (!(MCG->S & MCG_S_OSCINIT0_MASK));
    while (MCG->S & MCG_S_IREFST_MASK);
    while ((MCG->S & MCG_S_CLKST_MASK) != 8u);

    // FBE -> PBE: Enable PLL (8MHz / 2 * 30 = 120MHz)
    MCG->C5 = MCG_C5_PRDIV0(1u);
    MCG->C6 = MCG_C6_PLLS_MASK |
              MCG_C6_VDIV0(6u);

    while (!(MCG->S & MCG_S_PLLST_MASK));
    while (!(MCG->S & MCG_S_LOCK0_MASK));

    // PBE -> PEE: Switch core to PLL output
    MCG->C1 &= ~MCG_C1_CLKS_MASK;

    while ((MCG->S & MCG_S_CLKST_MASK) != 0x0C);
}
