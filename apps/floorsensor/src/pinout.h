/**
 * @file
 * This file configures the pin numbers of all
 * hardware attached to the nRF51,
 * e.g. where the sensors are connected
 * and optionally LED pins.
 */

#ifndef PINOUT_H
#define PINOUT_H

#include "platform.h"

/**
 * nRF pins to which the sensor outputs are connected
 */
enum pin_sensor_e
{
	PIN_SENSOR_1 = 30,
	PIN_SENSOR_2 = 1,
	PIN_SENSOR_3 = 3,
	PIN_SENSOR_4 = 4,
	PIN_SENSOR_5 = 2,

	PIN_SENSOR_6 = 13,
	PIN_SENSOR_7 = 11,
	PIN_SENSOR_8 = 9,
	PIN_SENSOR_9 = 7,
	PIN_SENSOR_10 = 5,

	PIN_SENSOR_11 = 6,
	PIN_SENSOR_12 = 8,
	PIN_SENSOR_13 = 10,
	PIN_SENSOR_14 = 12,
	PIN_SENSOR_15 = 14,
};

/**
 * Make the pin numbers of @ref pin_sensor_e available as an array
 */
#define SENSOR_PIN(index)  (const uint8_t[]) { \
	PIN_SENSOR_1, \
	PIN_SENSOR_2, \
	PIN_SENSOR_3, \
	PIN_SENSOR_4, \
	PIN_SENSOR_5, \
	PIN_SENSOR_6, \
	PIN_SENSOR_7, \
	PIN_SENSOR_8, \
	PIN_SENSOR_9, \
	PIN_SENSOR_10, \
	PIN_SENSOR_11, \
	PIN_SENSOR_12, \
	PIN_SENSOR_13, \
	PIN_SENSOR_14, \
	PIN_SENSOR_15 \
	}[index]


#define NRFDUINO_PIN_LED            28
#define ADAPTER_SMD_TH_PIN_LED      20

//#ifdef PLATFORM_NRFDUINO

// indicates BLE advertising
#define PIN_LED_ADVERTISING                 18

// indicates BLE connected
// active high
#define PIN_LED_CONNECTED                   19

#define PIN_LED_RECEIVE                     17

// indicates measurement cycle progressing
#define PIN_LED_MEASUREMENT_CYCLE_COMPLETE  16

#define PIN_DEBUG_MEASUREMENT_INTERVAL      15
//#endif

#if defined(PLATFORM_ADAPTER_SMD) || defined(PLATFORM_ADAPTER_TH)
#define PIN_LED_DEBUG                       ADAPTER_SMD_TH_PIN_LED
#endif

#define led_init()  \
        hal_gpio_init_out(PIN_LED_DEBUG, 1);
    /*
	    hal_gpio_init_out(PIN_LED_ADVERTISING, 1);  \
	    hal_gpio_init_out(PIN_LED_CONNECTED, 1);  \
	    hal_gpio_init_out(PIN_LED_RECEIVE, 1);  \
	    hal_gpio_init_out(PIN_LED_MEASUREMENT_CYCLE_COMPLETE, 1);  \
	    hal_gpio_init_out(PIN_DEBUG_MEASUREMENT_INTERVAL, 1);   \
	    */

#endif // PINOUT_H
