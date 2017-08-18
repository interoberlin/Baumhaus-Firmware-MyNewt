#ifndef PINOUT_H
#define PINOUT_H

// nRF pins to which the sensor outputs are connected
enum
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

// indicates BLE advertising
#define PIN_LED_ADVERTISING                 18

// indicates BLE connected
// active high
#define PIN_LED_CONNECTED                   19

#define PIN_LED_RECEIVE                     17

// indicates measurement cycle progressing
#define PIN_LED_MEASUREMENT_CYCLE_COMPLETE  20

#define PIN_DEBUG_MEASUREMENT_INTERVAL      15

#endif // PINOUT_H
