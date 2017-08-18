
#include "hal_gpiote.h"


void hal_gpio_cfg_sense_input(int pin, hal_gpio_sense_t sense)
{
    // Clear sense bits
    NRF_GPIO->PIN_CNF[pin] &= ~GPIO_PIN_CNF_SENSE_Msk;

    // Set sense bits
    NRF_GPIO->PIN_CNF[pin] |= sense;
}


void hal_gpiote_event_config(int event_channel, int pin, hal_gpio_edge_t edge_type)
{
    /* Configure the channel as the caller expects */
    NRF_GPIOTE->CONFIG[event_channel] = (GPIOTE_CONFIG_MODE_Event << GPIOTE_CONFIG_MODE_Pos)
                                      | (pin << GPIOTE_CONFIG_PSEL_Pos)
                                      |  edge_type;

    /* Three NOPs are required to make sure configuration is written before setting tasks or getting events */
    __NOP();
    __NOP();
    __NOP();

    /* Clear the event that appears in some cases */
    NRF_GPIOTE->EVENTS_IN[event_channel] = 0;
}
