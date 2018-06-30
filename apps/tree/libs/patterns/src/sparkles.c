
#include "../../../../tree/libs/patterns/include/patterns/sparkles.h"

extern sparkle_t pattern_sparkle[SPARKLE_COUNT];

inline void init_sparkle(sparkle_t *sparkle)
{
    // select a random LED
    bool led_is_occupied;
    uint8_t new_led_index;
    uint8_t new_strip_index;
    do
    {
        new_led_index  = rand() % 10;
        new_strip_index = rand() % 8;

        led_is_occupied = false;
        for (uint8_t i=0; i<SPARKLE_COUNT; i++)
        {
            if (pattern_sparkle[i].led.strip_index == new_strip_index
             && pattern_sparkle[i].led.led_index == new_led_index)
             {
                led_is_occupied = true;
                break;
             }
        }
    } while (led_is_occupied);

    sparkle->led.strip_index = new_strip_index;
    sparkle->led.led_index = new_led_index;

    // reset time
    sparkle->t = 0;

    // set a random fade-in time
    sparkle->t_fade_in = 5 + rand() % 10;

    sparkle->duration_fade_in = 5 + (rand() % 15);

    // set fade-out time after fade-in time
    sparkle->t_fade_out = sparkle->t_fade_in + sparkle->duration_fade_in;

    sparkle->duration_fade_out = 5 + (rand() % 20);

    // set end time to when fade-out is complete
    sparkle->t_end = sparkle->t_fade_out + sparkle->duration_fade_out;

    sparkle->color.simple.ww = rand();
    sparkle->color.simple.cw = rand() % 25;
    sparkle->color.simple.a = rand();
}

inline void sparkle_update(sparkle_t *sparkle)
{
    uint8_t ww = 0;
    uint8_t cw = 0;
    uint8_t a = 0;

    if (sparkle->t >= sparkle->t_end)
        ;
    else if (sparkle->t >= sparkle->t_fade_out)
    {
        // fade out
        ww = sparkle->color.simple.ww - (sparkle->t - sparkle->t_fade_out) * sparkle->color.simple.ww/sparkle->duration_fade_out;
        cw = sparkle->color.simple.cw - (sparkle->t - sparkle->t_fade_out) * sparkle->color.simple.cw/sparkle->duration_fade_out;
        a = sparkle->color.simple.a - (sparkle->t - sparkle->t_fade_out) * sparkle->color.simple.a/sparkle->duration_fade_out;
    }
    else if (sparkle->t >= sparkle->t_fade_in)
    {
        // fade in
       ww = (sparkle->t - sparkle->t_fade_in) * sparkle->color.simple.ww/sparkle->duration_fade_in;
       cw = (sparkle->t - sparkle->t_fade_in) * sparkle->color.simple.cw/sparkle->duration_fade_in;
       a = (sparkle->t - sparkle->t_fade_in) * sparkle->color.simple.a/sparkle->duration_fade_in;
    }

    set_led(sparkle->led, ww, cw, a);
}

inline bool is_sparkle_finished(sparkle_t *sparkle)
{
    return (sparkle->t > sparkle->t_end);
}
