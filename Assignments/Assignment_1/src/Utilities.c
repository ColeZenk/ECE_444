/**
 * @file   Utilities.c
 * @author Cole Zenk
 * @breif  The best tools are the simple ones that work...
 *         On that not, heres a file with a bunch of simple tools that work!
 */

#include "MK22F51212.h"
#include "Utilities.h"

#include <math.h>

/*************************************************************************************************
  Some UART funtions for debugging
**************************************************************************************************/

void UART0_Init(void) {
    // Enable clocks
    SIM->SCGC4 |= SIM_SCGC4_UART0_MASK;
    SIM->SCGC5 |= SIM_SCGC5_PORTB_MASK;

    // Set PTB17 as UART0_TX (ALT3)
    PORTB->PCR[17] = PORT_PCR_MUX(3);

    // Disable UART while configuring
    UART0->C2 = 0;

    // 120MHz clock, 115200 baud: SBR = 120000000 / (16 * 115200) = 65
    UART0->BDH = 0;
    UART0->BDL = 65;

    // Enable transmitter
    UART0->C2 = UART_C2_TE_MASK;
}

void UART0_PutChar(char c) {
    while (!(UART0->S1 & UART_S1_TDRE_MASK));
    UART0->D = c;
}

void UART0_Print(const char *s) {
    while (*s)
        UART0_PutChar(*s++);
}

/*************************************************************************************************
  Wave gen functions
**************************************************************************************************/

void WaveGen_Init(WaveGen_t *gen, float freq_hz, float amplitude, float offset) {
   gen->phase = 0.0f;
   gen->phase_increment = 2.0f * M_PI * freq_hz / FS_HZ;
   gen->amplitude = amplitude;
   gen->offset = offset;
}

float WaveGen_Sine(WaveGen_t *gen) {
    float value = gen->offset + gen->amplitude * sinf(gen->phase);
    gen->phase += gen->phase_increment;
    if (gen->phase >= 2.0f * M_PI) gen->phase -= 2.0f * M_PI;
    return value;
}

float WaveGen_Triangle(WaveGen_t *gen) {
    // Normalize phase to [0, 1)
    float t = gen->phase / (2.0f * M_PI);

    // Triangle: 0→1→0
    float tri = (t < 0.5f) ? (4.0f * t) : (4.0f - 4.0f * t);
    tri = tri - 1.0f;  // Now [-1, 1]

    float value = gen->offset + gen->amplitude * tri;
    gen->phase += gen->phase_increment;
    if (gen->phase >= 2.0f * M_PI) gen->phase -= 2.0f * M_PI;
    return value;
}

float WaveGen_Square(WaveGen_t *gen) {
    float value = gen->offset + gen->amplitude * ((gen->phase < M_PI) ? 1.0f : -1.0f);
    gen->phase += gen->phase_increment;
    if (gen->phase >= 2.0f * M_PI) gen->phase -= 2.0f * M_PI;
    return value;
}
