/*
 * Originally developed by Lava, LLC.
 * Original repository: https://github.com/lavallc/nrf51-neopixel
 * License: The MIT License
 *
 * This is a derivative work by Matthias Bock
 * to control SK6812-WWA LED strips from a nRF51 microcontroller
 * License: GNU GPL v3
 */

#ifndef NEOPIXEL_H
#define NEOPIXEL_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
//#include <hal/hal_delay.h>
#include <nrf51.h>
#include <hal/hal_gpio.h>


//These defines are timed specific to a series of if statements and will need to be changed
//to compensate for different writing algorithms than the one in neopixel.c
#define NEOPIXEL_SEND_ONE   NRF_GPIO->OUTSET = (1UL << PIN); \
        __ASM ( \
                " NOP\n\t" \
                " NOP\n\t" \
                " NOP\n\t" \
                " NOP\n\t" \
                " NOP\n\t" \
                " NOP\n\t" \
            ); \
        NRF_GPIO->OUTCLR = (1UL << PIN); \
        __ASM ( \
                " NOP\n\t" \
                " NOP\n\t" \
                " NOP\n\t" \
                " NOP\n\t" \
                " NOP\n\t" \
                " NOP\n\t" \
            ); \

#define NEOPIXEL_SEND_ZERO NRF_GPIO->OUTSET = (1UL << PIN); \
        NRF_GPIO->OUTCLR = (1UL << PIN);  \
        __ASM ( \
                " NOP\n\t" \
                " NOP\n\t" \
                " NOP\n\t" \
                " NOP\n\t" \
                " NOP\n\t" \
                " NOP\n\t" \
            );

extern void nrf_delay_us(uint32_t volatile number_of_us);

typedef union
{
    struct
    {
        uint8_t cw, a, ww;
    } simple;
    uint8_t cwa[3];
} color_t;

typedef struct
{
    color_t color;
//  uint8_t pattern[8];
} led_t;

typedef struct
{
    uint8_t     pin_num;
    uint16_t    num_leds;
    color_t*    leds;
} neopixel_strip_t;

/**
  @brief Initialize GPIO and data location
  @param[in] pointer to Strip structure
    @param[in] pin number for GPIO
*/
void neopixel_init(neopixel_strip_t *strip, uint8_t pin_num, uint16_t num_leds);

/**
  @brief Turn all LEDs off
  @param[in] pointer to Strip structure
*/
void neopixel_clear(neopixel_strip_t *strip);

/**
  @brief Update strip with structure data
  @param[in] pointer to Strip structure
*/
void neopixel_show(neopixel_strip_t *strip);

/**
 * @brief Get the current color of an LED
 * @param[in] strip: the strip, this LED is on
 * @param[in] index: index of the LED on the strip
 * @param[out] warmwhite: warmwhite color value of this LED
 * @param[out] coldwhite: coldwhite color value of this LED
 * @param[out] amber: amber color value of this LED
 */
void neopixel_get_color(neopixel_strip_t* strip, uint16_t index, uint8_t* warmwhite, uint8_t* coldwhite, uint8_t* amber);

/**
  @brief Write RGB value to LED structure
  @param[in] pointer to strip structure
  @param[in] LED number (starting at 1)
  @retval 0 Successful write
  @retval 1 LED number is out of bounds
*/
void neopixel_set_color(neopixel_strip_t *strip, uint16_t index, uint8_t warmwhite, uint8_t coldwhite, uint8_t amber);


/**
  @brief Write RGB value to LED structure and update LED
  @param[in] pointer to strip structure
  @param[in] LED number (starting at 1)
  @retval 0 Successful write
  @retval 1 LED number is out of bounds
*/
void neopixel_set_color_and_show(neopixel_strip_t *strip, uint16_t index, uint8_t warmwhite, uint8_t coldwhite, uint8_t amber);

/**
  @brief Clears structure data
  @param[in] pointer to Strip structure
*/
void neopixel_destroy(neopixel_strip_t *strip);

#endif
