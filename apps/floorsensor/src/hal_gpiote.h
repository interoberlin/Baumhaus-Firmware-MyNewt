/**
 * Library for the GPIO tasks and events functionality
 */

#ifndef GPIOTE_H
#define GPIOTE_H

#include <nrf51.h>
#include <nrf51_bitfields.h>
#include <hal/hal_gpio.h>


typedef enum
{
    HAL_GPIO_SENSE_HIGH = (GPIO_PIN_CNF_SENSE_High << GPIO_PIN_CNF_SENSE_Pos),
    HAL_GPIO_SENSE_LOW  = (GPIO_PIN_CNF_SENSE_Low << GPIO_PIN_CNF_SENSE_Pos),
} hal_gpio_sense_t;

typedef enum
{
    HAL_GPIOTE_EDGE_RISING  = (GPIOTE_CONFIG_POLARITY_LoToHi << GPIOTE_CONFIG_POLARITY_Pos),
    HAL_GPIOTE_EDGE_FALLING = (GPIOTE_CONFIG_POLARITY_HiToLo << GPIOTE_CONFIG_POLARITY_Pos),
    HAL_GPIOTE_EDGE_BOTH    = (GPIOTE_CONFIG_POLARITY_Toggle << GPIOTE_CONFIG_POLARITY_Pos),
} hal_gpio_edge_t;


void hal_gpio_cfg_sense_input(int pin, hal_gpio_sense_t sense);

void hal_gpiote_event_config(int event_channel, int pin, hal_gpio_edge_t edge_type);

#endif // GPIOTE_H
