#ifndef WARBLINGWIRE_H
#define WARBLINGWIRE_H

#include <math.h>
#define NORMALIZE_FREQ_CONST       2 * M_PI / 10000
/**************************************************************************
  TWEAKING VALUES
 **************************************************************************/

/**
 * @macros  Functional options
 * @purpose Through this macro you can define the ADC channel used.
 */
#define ADC_CHANNEL                12
#define FREQUENCY_WARBLE_HZ        3 * NORMALIZE_FREQ_CONST

/**************************************************************************
  GLOBAL FUNCTIONS (interfaces)
 **************************************************************************/

/*
 * @function WarblingWire
 * @purpose  A simple interfacing function allowing the warbling wire algorithm to be enabled. This
 *           architecture will reduce coupling, increase readability, while maintaining proper
 *           coheision.
 */
void WarbleWire(void);

// The init function to boot.
void WarblingWire_Init(void);


/**************************************************************************
  TESTING
 **************************************************************************/

/**
 * @macros   Test Procedure
 * @purpose  Comment or uncomment to disable certain features for clean encapsulated tests that
 *           enforce minimal coupling.
 */

/* #define TEST_PASSTHROUGH */
/* #define TEST_OUTPUT */

/* Output test constants */
#ifdef TEST_OUTPUT

#define TEST_SIGNAL_FREQ_HZ     100.0f
#define TEST_PERIOD             30000

typedef enum {
    WAVEFORM_SINE,
    WAVEFORM_TRIANGLE,
    WAVEFORM_SQUARE,

    WAVEFORM_COUNT
} Waveform_t;

#endif

#if defined(TEST_PASSTHROUGH) || defined(TEST_OUTPUT)
    #define TEST_MODE
    #warning "Test mode enabled"
#endif

#if defined(TEST_PASSTHROUGH) && defined(TEST_OUTPUT)
#define TEST_MODE
    #error "Multiple test modes enabled simultaneously"
#endif

#endif //WARBLINGWIRE_H
