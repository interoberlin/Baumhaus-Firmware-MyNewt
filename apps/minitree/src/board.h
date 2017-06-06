/**
 * Board-related routines
 */

#ifndef BOARD_H
#define BOARD_H

#include <hal/hal_gpio.h>
#include <pinout.h>

// for development/debugging
#ifdef BOARD_NRFDUINO
#include <nrfduino.h>
#endif

/**
 * Initialize GPIO pins
 */
#ifdef MINI_TREE
#define init_gpio_tree_adapter() \
    nrf_gpio_cfg_output(PIN_LED_DATA0); \
    nrf_gpio_cfg_output(PIN_LED_DATA1); \
    nrf_gpio_cfg_output(PIN_LED_DATA2); \
    nrf_gpio_cfg_output(PIN_LED_DATA3); \
    nrf_gpio_cfg_output(PIN_LED_DATA4); \
    nrf_gpio_cfg_output(PIN_LED_DATA5); \
    nrf_gpio_cfg_output(PIN_LED_DATA6); \
    nrf_gpio_cfg_output(PIN_LED_DATA7);
#else
#define init_gpio_tree_adapter() \
    nrf_gpio_cfg_output(PIN_LED_DATA1); \
    nrf_gpio_cfg_output(PIN_LED_DATA2); \
    nrf_gpio_cfg_output(PIN_LED_DATA3); \
    nrf_gpio_cfg_output(PIN_LED_DATA4); \
    \
    nrf_gpio_cfg_output(PIN_ATX_ON); \
    nrf_gpio_pin_clear(PIN_ATX_ON); \
    \
    nrf_gpio_cfg_input(PIN_FUSE_OK, NRF_GPIO_PIN_NOPULL); \
    \
    nrf_gpio_cfg_input(PIN_CURRENT_SENSOR, NRF_GPIO_PIN_NOPULL);
#endif // MINI_TREE


#ifdef BOARD_NRFDUINO
#define init_gpio() \
    nrf_gpio_cfg_output(NRFDUINO_PIN_LED); \
    init_gpio_tree_adapter();
#else
#define init_gpio() \
    init_gpio_tree_adapter();
#endif // BOARD_NRFDUINO

/*
 * Switch the ATX power supply on/off
 */
#ifndef MINI_TREE
#define atx_powersupply_enable()    nrf_gpio_pin_set(PIN_ATX_ON);
#define atx_powersupply_disable()   nrf_gpio_pin_clear(PIN_ATX_ON);
#endif // MINI_TREE

/*
 * The pin assignment and LED count may vary on different sides of the tree
 */
#ifdef MINI_TREE
    #define LEDSTRIP_COUNT  8
    #define TOTAL_LED_COUNT 80
#else
    #ifdef TREE_SIDE_SOUTH
        #define PIN_LEDSTRIP_TOP_LEFT       PIN_LED_DATA1
        #define PIN_LEDSTRIP_TOP_RIGHT      PIN_LED_DATA2
        #define PIN_LEDSTRIP_BOTTOM_LEFT    PIN_LED_DATA3
        #define PIN_LEDSTRIP_BOTTOM_RIGHT   PIN_LED_DATA4
        #define LED_COUNT_TOP_LEFT          150
        #define LED_COUNT_TOP_RIGHT         150
        #define LED_COUNT_BOTTOM_LEFT       150
        #define LED_COUNT_BOTTOM_RIGHT      150
    #endif // TREE_SIDE_SOUTH

    #define LEDSTRIP_COUNT      4
    #define LED_COUNT_LEFT      (LED_COUNT_TOP_LEFT + LED_COUNT_BOTTOM_LEFT)
    #define LED_COUNT_RIGHT     (LED_COUNT_TOP_RIGHT + LED_COUNT_BOTTOM_RIGHT)
    #define TOTAL_LED_COUNT     (LED_COUNT_LEFT + LED_COUNT_RIGHT)
#endif // MINI_TREE

#endif // BOARD_H
