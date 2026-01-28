/**
 * @file WarblingWire.c
 * @brief Warbling Digital Wire - ECE 444 HW1
 * @author Cole Zenk
 *
 * Implements a digital passthrough with amplitude modulation (warble).
 * Samples ADC at 10kHz, applies envelope, outputs to DAC.
 *
 * @scope
 * File coverers encapsulated implementation of the "algorithm" as well as some minimal
 *      unit testing and verification methods.
 */

/*************************************************************************
 INCLUDES
 *************************************************************************/
#include <math.h>
#include <stdbool.h>

#include "MK22F51212.h"
#include "TimerInt.h"


#include "WarblingWire.h"

#ifdef TEST_OUTPUT
#include "Utilities.h"

// Some floating point arithmatic
#define  BITS                 (12u)
#define  BIT_RESOLUTION       (float)(1u << BITS)            // 2 raised to BITS
#define  VREFH                (3.0f)
#define  ADC_SCALE            (BIT_RESOLUTION / VREFH)

WaveGen_t test_osc;

static uint16_t outputTest(void);
#endif

/*************************************************************************
 FILE SCOPED FUNCTIONS
 *************************************************************************/
static uint16_t warbleTransform(uint16_t input);
static uint16_t processInput(uint16_t adc_sample);


/*************************************************************************
 CORE ALGORITHM
 *************************************************************************/
static uint16_t warbleTransform(uint16_t input){
    volatile uint16_t transformed_output;
    static float accumulated_phase = 0.0f;

    // Envelope range: 10% to 100% scale
    static const float min = 0.1f;
    static const float max = 1.0f;
    static const float amplitude = (max - min) / 2.0f;    // +/- 0.45
    static const float offset    = (max + min) / 2.0f;    // 0.55 center


    // Calculations
    accumulated_phase +=  (float)FREQUENCY_WARBLE_HZ;
    float envelope = offset + amplitude * sinf(accumulated_phase);   //0.1 to 1.0

    transformed_output = (uint16_t)(input * envelope);
    return transformed_output;
}

/*************************************************************************
 GLOBAL INTERFACE
 ************************************************************************/
void WarbleWire(void){
    volatile uint16_t adc_sample;
    volatile uint16_t output = 0;

    ADC0->SC1[0] = ADC_SC1_ADCH(ADC_CHANNEL);  //  ADC0_SE12 (defined as PTB2-12 in ref-man 10.3)
    adc_sample = ADC0->R[0];
    output = processInput(adc_sample);

    // Write to DAC (12-bit)
    DAC0->DAT[0].DATL = output & 0xFF;
    DAC0->DAT[0].DATH = (output >> 8) & 0x0F;
}

void PIT0_IRQHandler(void) {
    PIT->CHANNEL[0].TFLG = PIT_TFLG_TIF_MASK;
    WarbleWire();
}

void WarblingWire_Init(void){
   TimerInt_Init();
   ADC0->SC1[0] = ADC_SC1_ADCH(ADC_CHANNEL);  // Prime the ISR
}

/*************************************************************************
 FILE SCOPED IMPLEMENTATIONS
 *************************************************************************/
static uint16_t processInput(uint16_t adc_sample){

#ifndef TEST_MODE

    return warbleTransform(adc_sample);

#elif  defined(TEST_PASSTHROUGH)
    return adc_sample;
#elif  defined(TEST_OUTPUT)
    adc_sample = outputTest();
    return adc_sample;
#endif
}

/*************************************************************************
 TESTING
 *************************************************************************/
#ifdef TEST_OUTPUT

static uint16_t outputTest(void){
    volatile uint16_t testOutput = 0;
    static uint16_t   count = 0;
    static Waveform_t waveformToTest = 0;

    static bool first = true;
    if(first){
        WaveGen_Init(&test_osc, TEST_SIGNAL_FREQ_HZ, 1.5f * ADC_SCALE, 1.5f * ADC_SCALE);
        first = false;
    }

    switch(waveformToTest){
        case WAVEFORM_SINE:
             testOutput = warbleTransform((uint16_t)WaveGen_Sine(&test_osc));
             break;
        case WAVEFORM_TRIANGLE:
             testOutput = warbleTransform((uint16_t)WaveGen_Triangle(&test_osc));
             break;
        case WAVEFORM_SQUARE:
             testOutput = warbleTransform((uint16_t)WaveGen_Square(&test_osc));
             break;
        default:
             waveformToTest = WAVEFORM_SINE;
             testOutput = warbleTransform((uint16_t)WaveGen_Sine(&test_osc));
             break;
    }

    if(count >= TEST_PERIOD){
        waveformToTest++;
        count = 0;
    } else count++;

    return testOutput;
}
#endif

