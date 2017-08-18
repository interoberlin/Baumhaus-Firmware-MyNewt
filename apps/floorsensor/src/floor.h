#ifndef FLOOR_H
#define FLOOR_H

#include <stdbool.h>
#include <stdint.h>

#include <nrf51.h>

/*
 * ------------------------------------------------
 *  Configuration
 * ------------------------------------------------
 */
#define FLOOR_USES_TIMER2
#define PULSE_COUNTER      NRF_TIMER1
#define TIMER_MEASUREMENT  NRF_TIMER2

// the index of the last sensor
// counting start with 1
#define SENSOR_COUNT 15

/*
 * timer overflow values as multiples of clock ticks (depends on fTIMER i.e. PRESCALER value)
 * measurement duration: 10 ms
 * measurement interval: 13 ms / 77 Hz
 */ 
// in kHz because below we calculate with milliseonds
#define BASE_CLOCK (float) 16000
//#define measurement_duration 1250 // 10ms @PRESCALER=7
//#define measurement_interval 1625 // 13ms @PRESCALER=7
//#define measurement_duration 2500 // 10ms @PRESCALER=6
//#define measurement_interval 3250 // 13ms @PRESCALER=6
#define MEASUREMENT_PRESCALER       6
#define MEASUREMENT_DURATION_MS     10
#define MEASUREMENT_DURATION_TICKS  (uint32_t) (MEASUREMENT_DURATION_MS / ((1 << MEASUREMENT_PRESCALER) / BASE_CLOCK))
#define MEASUREMENT_INTERVAL_MS     50
#define MEASUREMENT_INTERVAL_TICKS  (uint32_t) (MEASUREMENT_INTERVAL_MS / ((1 << MEASUREMENT_PRESCALER) / BASE_CLOCK))

// ------------------------------------------------

/**
 * Prepare timers and counters for floor sensor measurement
 */
void floor_init();


/**
 * Prepare a timer peripheral
 * to throw an interrupt,
 * both when a measurement shall be started
 * and when it shall be stopped
 */
void floor_timer_init();

/**
 * Start the floor measurement scheduling timer
 */
void floor_timer_enable();

/**
 * Stop the floor measurement scheduling timer
 */
void floor_timer_disable();

#endif // FLOOR_H
