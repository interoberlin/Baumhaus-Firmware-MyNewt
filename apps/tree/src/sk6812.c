/**
 * Example code:
 
    neopixel_strip_t m_strip;
    uint8_t dig_pin_num = 6;
    uint8_t leds_per_strip = 24;
    uint8_t error;
    uint8_t led_to_enable = 10;
    uint8_t red = 255;
    uint8_t green = 0;
    uint8_t blue = 159;

    neopixel_init(&m_strip, dig_pin_num, leds_per_strip);
    neopixel_clear(&m_strip);
    error = neopixel_set_color_and_show(&m_strip, led_to_enable, red, green, blue);
    if (error) {
        //led_to_enable was not within number leds_per_strip
    }
    //clear and remove strip
    neopixel_clear(&m_strip);
    neopixel_destroy(&m_strip);
 
 
 * For use with BLE stack, see information below:
    - Include in main.c
        #include "ble_radio_notification.h"
    - Call (see nrf_soc.h: NRF_RADIO_NOTIFICATION_DISTANCES and NRF_APP_PRIORITIES)
        ble_radio_notification_init(NRF_APP_PRIORITY_xxx,
                                    NRF_RADIO_NOTIFICATION_DISTANCE_xxx,
                                    your_radio_callback_handler);
    - Create
        void your_radio_callback_handler(bool radio_active)
        {
            if (radio_active == false)
            {
                neopixel_show(&strip1);
                neopixel_show(&strip2);
                //...etc
            }
        }
    - Do not use neopixel_set_color_and_show(...) with BLE, instead use uint8_t neopixel_set_color(...);
 */
 
#include "../../tree/src/sk6812.h"

void neopixel_init(neopixel_strip_t *strip, uint8_t pin_num, uint16_t num_leds)
{
// TODO: malloc appears not to be working with my Makefile / linker script, only with Nordic's
//    strip->leds = (color_t*) malloc(sizeof(color_t) * num_leds);

    strip->pin_num  = pin_num;
    strip->num_leds = num_leds;

    //nrf_gpio_cfg_output(pin_num);
    hal_gpio_init_out(pin_num, 0);

    //NRF_GPIO->OUTCLR = (1UL << pin_num);

    neopixel_clear(strip);
}

void neopixel_clear(neopixel_strip_t* strip)
{
    // initialize LED colors with zeroes
    for (int i = 0; i < strip->num_leds; i++)
    {
        strip->leds[i].simple.ww = 0;
        strip->leds[i].simple.cw = 0;
        strip->leds[i].simple.a  = 0;
    }
}

void neopixel_show(neopixel_strip_t *strip)
{
    const uint8_t PIN =  strip->pin_num;

    // transmit reset
    //NRF_GPIO->OUTCLR = (1UL << PIN);
    //nrf_delay_us(80);

    // transmit all LED values
    for (int i = 0; i < strip->num_leds; i++)
    {
        // transmit one color after the other
        for (int j = 0; j < 3; j++)
        {
            if ((strip->leds[i].cwa[j] & 128) > 0)
                {NEOPIXEL_SEND_ONE}
            else
                {NEOPIXEL_SEND_ZERO}

            if ((strip->leds[i].cwa[j] & 64) > 0)
                {NEOPIXEL_SEND_ONE}
            else
                {NEOPIXEL_SEND_ZERO}

            if ((strip->leds[i].cwa[j] & 32) > 0)
                {NEOPIXEL_SEND_ONE}
            else
                {NEOPIXEL_SEND_ZERO}

            if ((strip->leds[i].cwa[j] & 16) > 0)
                {NEOPIXEL_SEND_ONE}
            else
                {NEOPIXEL_SEND_ZERO}

            if ((strip->leds[i].cwa[j] & 8) > 0)
                {NEOPIXEL_SEND_ONE}
            else
                {NEOPIXEL_SEND_ZERO}

            if ((strip->leds[i].cwa[j] & 4) > 0)
                {NEOPIXEL_SEND_ONE}
            else
                {NEOPIXEL_SEND_ZERO}

            if ((strip->leds[i].cwa[j] & 2) > 0)
                {NEOPIXEL_SEND_ONE}
            else
                {NEOPIXEL_SEND_ZERO}

            if ((strip->leds[i].cwa[j] & 1) > 0)
                {NEOPIXEL_SEND_ONE}
            else
                {NEOPIXEL_SEND_ZERO}
        }
    }
}

void neopixel_get_color(neopixel_strip_t* strip, uint16_t index, uint8_t* warmwhite, uint8_t* coldwhite, uint8_t* amber)
{
    if (index < strip->num_leds)
    {
        *warmwhite = strip->leds[index].simple.ww;
        *coldwhite = strip->leds[index].simple.cw;
        *amber     = strip->leds[index].simple.a;
    }
}

void neopixel_set_color(neopixel_strip_t* strip, uint16_t index, uint8_t warmwhite, uint8_t coldwhite, uint8_t amber)
{
    if (index < strip->num_leds)
    {
        strip->leds[index].simple.ww = warmwhite;
        strip->leds[index].simple.cw = coldwhite;
        strip->leds[index].simple.a  = amber;
    }
}

void neopixel_set_color_and_show(neopixel_strip_t* strip, uint16_t index, uint8_t warmwhite, uint8_t coldwhite, uint8_t amber)
{
    if (index < strip->num_leds)
    {
/*
        strip->leds[index].simple.ww = warmwhite;
        strip->leds[index].simple.cw = coldwhite;
        strip->leds[index].simple.a  = amber;
*/
        strip->leds[index].cwa[0] = warmwhite;
        strip->leds[index].cwa[1] = coldwhite;
        strip->leds[index].cwa[2] = amber;

        neopixel_show(strip);
    }
}

void neopixel_destroy(neopixel_strip_t *strip)
{
    free(strip->leds);
    strip->num_leds = 0;
}
