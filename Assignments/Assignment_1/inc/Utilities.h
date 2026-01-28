#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdint.h>

#define FS_HZ 10000.0f

void UART0_Init(void);
void UART0_PutChar(char c);
void UART0_Print(const char *s);


typedef struct {
    float phase;           // Current phase [0, 2π)
    float phase_increment; // 2pi × freq / Fs
    float amplitude;       // Peak amplitude
    float offset;          // DC offset
} WaveGen_t;

                                            // Initialize oscillator
void WaveGen_Init(WaveGen_t *gen, float freq_hz, float amplitude, float offset);

// Get next sample (updates internal phase)
float WaveGen_Sine(WaveGen_t *gen);
float WaveGen_Triangle(WaveGen_t *gen);
float WaveGen_Square(WaveGen_t *gen);


#endif //UTILITIES_H
