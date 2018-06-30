
#include "../../tree/src/leds.h"

// four SK6812 LED strips
neopixel_strip_t strip[LEDSTRIP_COUNT];

// static memory allocation for LEDs
// to avoid the use of malloc
uint8_t led_memory[TOTAL_LED_COUNT*3];

// pack constants in arrays to allow for iteration over all strips
#ifndef MINI_TREE
const uint8_t strip_at_pin[LEDSTRIP_COUNT] =
{
    PIN_LEDSTRIP_TOP_LEFT,
    PIN_LEDSTRIP_TOP_RIGHT,
    PIN_LEDSTRIP_BOTTOM_LEFT,
    PIN_LEDSTRIP_BOTTOM_RIGHT
};
const uint8_t leds_per_strip[LEDSTRIP_COUNT] =
{
    LED_COUNT_TOP_LEFT,
    LED_COUNT_TOP_RIGHT,
    LED_COUNT_BOTTOM_LEFT,
    LED_COUNT_BOTTOM_RIGHT
};
#else
const uint8_t strip_at_pin[LEDSTRIP_COUNT] =
{
    PIN_LED_DATA0,
    PIN_LED_DATA1,
    PIN_LED_DATA2,
    PIN_LED_DATA3,
    PIN_LED_DATA4,
    PIN_LED_DATA5,
    PIN_LED_DATA6,
    PIN_LED_DATA7
};
const uint8_t leds_per_strip[LEDSTRIP_COUNT] =
{
    10,
    10,
    10,
    10,
    10,
    10,
    10,
    10
};
#endif // MINI_TREE

// remember, which strips were changed
volatile bool strip_changed[LEDSTRIP_COUNT];


void init_ledstrips()
{
    uint8_t *memptr =  led_memory;
    for (int strip_num=0; strip_num<LEDSTRIP_COUNT; strip_num++)
    {
        strip[strip_num].leds = (color_t*) memptr;
        memptr = memptr + leds_per_strip[strip_num]*3;
        neopixel_init(&strip[strip_num], strip_at_pin[strip_num], leds_per_strip[strip_num]);
        // neopixel_clear() may be unnecessary, should be filled with zeroes by startup script anyways
        neopixel_clear(&strip[strip_num]);
        strip_changed[strip_num] = true;
    }
}

#ifndef MINI_TREE
inline bool convert_led_address(
    uint16_t abstract_led_index,
    uint8_t *strip_index,
    uint16_t *led_index
    )
{
    // highest nibble selects column
    uint8_t led_column = abstract_led_index >> 12;
    // lower three nibbles select individual LED
    abstract_led_index &= 0x0FFF;

    if (led_column == LED_COLUMN_LEFT)
    {
        if (abstract_led_index < LED_COUNT_BOTTOM_LEFT)
        {
            *strip_index = 2; // bottom left
            *led_index = LED_COUNT_BOTTOM_LEFT - abstract_led_index;
            return true;
        }
        else if (abstract_led_index < LED_COUNT_BOTTOM_LEFT + LED_COUNT_TOP_LEFT)
        {
            *strip_index = 0; // top left
            *led_index = abstract_led_index - LED_COUNT_BOTTOM_LEFT;
        }
    }
    else if (led_column == LED_COLUMN_RIGHT)
    {
        if (abstract_led_index < LED_COUNT_BOTTOM_RIGHT)
        {
            *strip_index = 3; // bottom right
            *led_index = LED_COUNT_BOTTOM_RIGHT - abstract_led_index;
            return true;
        }
        else if (abstract_led_index < LED_COUNT_BOTTOM_RIGHT + LED_COUNT_TOP_RIGHT)
        {
            *strip_index = 1; // top right
            *led_index = abstract_led_index - LED_COUNT_BOTTOM_RIGHT;
        }
    }

    return false; // invalid column or led index
}
#endif // MINI_TREE

void set_led(led_index_t abstract_led_index, uint8_t warmwhite, uint8_t coldwhite, uint8_t amber)
{
    uint8_t strip_index = 0;
    uint16_t led_index = 0;

#ifndef MINI_TREE
    if (!convert_led_address(abstract_led_index, &strip_index, &led_index))
        return;
#else
    led_index = abstract_led_index.led_index;
    strip_index = abstract_led_index.strip_index;
#endif // MINI_TREE

    neopixel_set_color(&strip[strip_index], led_index, warmwhite, coldwhite, amber);
    strip_changed[strip_index] = true;
}

void update_leds()
{
    for (uint8_t strip_index=0; strip_index<LEDSTRIP_COUNT; strip_index++)
    {
        if (strip_changed[strip_index])
        {
            neopixel_show(&strip[strip_index]);
            strip_changed[strip_index] = false;
        }
    }
}
