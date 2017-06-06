
#include "patterns.h"

sparkle_t pattern_sparkle[SPARKLE_COUNT];

void init_patterns()
{
    for (uint8_t i=0; i<SPARKLE_COUNT; i++)
    {
        init_sparkle(&pattern_sparkle[i]);
    }
}

void update_patterns()
{
    for (uint8_t i=0; i<SPARKLE_COUNT; i++)
    {
        // time is running...
        pattern_sparkle[i].t++;

        // reset pattern, if finished
        if (is_sparkle_finished(&pattern_sparkle[i]))
        {
            init_sparkle(&pattern_sparkle[i]);
        }

        // calculate LED colors
        sparkle_update(&pattern_sparkle[i]);
    }
}
