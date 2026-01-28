/*
 * @file TimerInt.h
 * @brief PIT (Periodic Interrupt Timer) interface for K22F
 * @author Ethan Hettwer
 * @target Freescale K22F
 */

#include <stdint.h>

#ifndef __TIMERINT_H_
#define __TIMERINT_H_

#define BUS_CLOCK_HZ            60000000u
#define SAMPLE_RATE_HZ          10000u
#define TIMER_LOAD_COUNT        ((BUS_CLOCK_HZ / SAMPLE_RATE_HZ) - 1)

// No idea if this is crypticly used elsewhere...
// I'm going to stick with the responsible choice and just keep it around in case.
#define PIT_LDVAL               TIMER_LOAD_COUNT

void TimerInt_Init(void);

#endif
