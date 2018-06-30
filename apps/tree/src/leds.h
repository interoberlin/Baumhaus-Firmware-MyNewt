/**
 * LED configuration and functions
 * for operating on LED values
 */

#ifndef LEDS_H
#define LEDS_H

#include <stdint.h>
#include <stdbool.h>

#include "board.h"
#include "sk6812.h"

#define LED_COLUMN_LEFT     0
#define LED_COLUMN_RIGHT    1

#ifdef MINI_TREE
typedef struct
{
    uint8_t strip_index;
    uint8_t led_index;
} led_index_t;
#else
typedef uint16_t led_index_t;
#endif // MINI_TREE

/**
 * @brief Initializes all LED strips
 */
void init_ledstrips();

/**
 * @brief Set the value of a specific LED
 */
void set_led(led_index_t, uint8_t, uint8_t, uint8_t);

/**
 * @brief Output LED values from RAM to LEDs
 */
void update_leds();

#endif // LEDS_H
