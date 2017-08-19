
#include "floor.h"

#include <nrf51_bitfields.h>
#include <cmsis_nvic.h>

#include <hal/hal_gpio.h>
#include "hal_gpiote.h"

#include "pinout.h"

/**
 * Bitmask of sensors connected to this adapter
 */
volatile uint16_t sensor_connected;

/**
 * Bitmask of sensors enabled for measuring
 */
volatile uint16_t sensor_enabled;

/**
 * Array of pulse counter values
 */
volatile uint16_t sensor_values[SENSOR_COUNT];

/**
 * Trigger threshold for sensor values
 */
volatile uint16_t sensor_threshold[SENSOR_COUNT];

/**
 * Bitmask of triggered sensors
 */
volatile uint16_t sensor_triggered;


/**
 * Index of the currently measured sensor
 */
uint8_t index_sensor_currently_measured = 0;


// Forward declarations for floor_init();
void floor_pulsecounter_init();
void floor_timer_init();


/**
 * Prepare timers and counters
 * for the measurement
 */
void floor_init()
{
    floor_pulsecounter_init();
    floor_timer_init();
}


/**
 * Use GPIO Tasks and Events (GPIOTE) channel 0
 * to generate an event, when the selected pin encounters a rising edge
 */
static inline void configure_pin_for_counting(uint8_t pin)
{
    hal_gpio_init_in(pin, HAL_GPIO_PULL_NONE);
    hal_gpio_cfg_sense_input(pin, HAL_GPIO_SENSE_LOW);
    hal_gpiote_event_config(0, pin, HAL_GPIOTE_EDGE_RISING);
}


/**
 * Set the currently selected sensor to be the first one
 */
static inline void select_first_sensor()
{
    index_sensor_currently_measured = 0;
    configure_pin_for_counting(SENSOR_PIN(0));
}


/**
 * Determine, whether the current sensor
 * is the last sensor or not
 */
static inline bool is_last_sensor()
{
    return (index_sensor_currently_measured >= SENSOR_COUNT);
}


/**
 * Select the next sensor to be measured
 */
static inline void select_next_sensor()
{
    index_sensor_currently_measured++;

    if (is_last_sensor())
    {
        select_first_sensor();
    }
    else
    {
        configure_pin_for_counting(SENSOR_PIN(index_sensor_currently_measured));
    }
}


/**
 * Prepare a counter peripheral for usage as pulse counter and
 * use Programmable Peripheral Interconnect (PPI) channels 0 and 1
 * to connect rising edges on a pin to pulse counter input
 */
void floor_pulsecounter_init()
{
    // set the timer to counter mode
    PULSE_COUNTER->MODE = TIMER_MODE_MODE_Counter;
    // set timer bit resolution
    PULSE_COUNTER->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
    // no shortcuts
    PULSE_COUNTER->SHORTS = 0;

    // PPI setup:
    // pin toggled => increment counter
    NRF_PPI->CH[0].EEP = (uint32_t) (&(NRF_GPIOTE->EVENTS_IN[0]));
    NRF_PPI->CH[0].TEP = (uint32_t) (&(PULSE_COUNTER->TASKS_COUNT));
    NRF_PPI->CHEN |= (PPI_CHEN_CH0_Enabled << PPI_CHEN_CH0_Pos);

    // PPI setup:
    // measurement complete => capture counter value
    NRF_PPI->CH[1].EEP = (uint32_t)&TIMER_MEASUREMENT->EVENTS_COMPARE[0];
    NRF_PPI->CH[1].TEP = (uint32_t)&PULSE_COUNTER->TASKS_CAPTURE[0];
    NRF_PPI->CHEN |= (PPI_CHEN_CH1_Enabled << PPI_CHEN_CH1_Pos);
}


static inline void restart_pulse_counter()
{
    // clear counter value
    PULSE_COUNTER->TASKS_CLEAR = 1;
    // enable counting
    PULSE_COUNTER->TASKS_START = 1;
}


static inline void stop_pulse_counter()
{
    // stop counting
    PULSE_COUNTER->TASKS_STOP = 1;
}


static inline uint16_t get_pulse_count()
{
    // request counter value to register
    //PULSE_COUNTER->TASKS_CAPTURE[0] = 1; // value capturing has been moved to PPI
    return PULSE_COUNTER->CC[0];
}


// Forward declaration for floor_timer_init();
void measurement_timer_irq_handler();


void floor_timer_init()
{
    // set the timer to Counter Mode
    TIMER_MEASUREMENT->MODE = TIMER_MODE_MODE_Timer;
    // set timer bit resolution
    TIMER_MEASUREMENT->BITMODE = TIMER_BITMODE_BITMODE_16Bit;
    /*
     * set prescaler: 0-9
     * fTIMER = 16 MHz / (2**PRESCALER)
     *
     * 16 MHz / 2**3 = 2 MHz
     * 16 MHz / 2**9 = 31250 Hz
     * 16 MHz / 2**7 = 125 kHz
     * 16 MHz / 2**6 = 250 kHz
     */
    TIMER_MEASUREMENT->PRESCALER = MEASUREMENT_PRESCALER;
    // no shortcuts
    TIMER_MEASUREMENT->SHORTS = 0;
    // clear the task first to be usable for later
    TIMER_MEASUREMENT->TASKS_CLEAR = 1;

    // set timer compare values
    TIMER_MEASUREMENT->CC[0] = MEASUREMENT_DURATION_TICKS;
    TIMER_MEASUREMENT->CC[1] = MEASUREMENT_INTERVAL_TICKS;

    // enable interrupt upon compare event
    TIMER_MEASUREMENT->INTENSET = (TIMER_INTENSET_COMPARE0_Enabled << TIMER_INTENSET_COMPARE0_Pos)
                                | (TIMER_INTENSET_COMPARE1_Enabled << TIMER_INTENSET_COMPARE1_Pos);

    // configure debug pin as output
    hal_gpio_init_out(PIN_DEBUG_MEASUREMENT_INTERVAL, 0);

    // manual setting for testing
    NVIC_SetPriority(TIMER2_IRQn, 1);
    NVIC_SetVector(TIMER2_IRQn, (uint32_t) &measurement_timer_irq_handler);
    NVIC_EnableIRQ(TIMER2_IRQn);
}


void floor_timer_enable()
{
    // reset timer
    TIMER_MEASUREMENT->TASKS_CLEAR = 1;
    // start timer
    TIMER_MEASUREMENT->TASKS_START = 1;
}


void floor_timer_disable()
{
    // stop timer
    TIMER_MEASUREMENT->TASKS_STOP = 1;
}


extern void floor_measurement_complete_handler();

/**
 * Interrupts Service Routine (ISR) for the timer,
 * which we use to determine measurement duration
 * and measurement interval
 */
void measurement_timer_irq_handler()
{
    /*
     * Measurement complete:
     * The counter value has been captured (by PPI)
     */
    if (TIMER_MEASUREMENT->EVENTS_COMPARE[0])  // it's a channel 1 compare event
    {
        // clear this event
        TIMER_MEASUREMENT->EVENTS_COMPARE[0] = 0;

        // stop pulse counter
        stop_pulse_counter();

        hal_gpio_write(PIN_DEBUG_MEASUREMENT_INTERVAL, 0);

        sensor_values[index_sensor_currently_measured] = get_pulse_count();

        // report results every 5 sensors
        if (index_sensor_currently_measured % SENSOR_COUNT == 0)
        {
            // invoke external event handler
            floor_measurement_complete_handler();
        }

        index_sensor_currently_measured = (index_sensor_currently_measured + 1) % SENSOR_COUNT;
    }

    /*
     * Measurement interval reached:
     * Begin new measurement cycle
     */
    if (TIMER_MEASUREMENT->EVENTS_COMPARE[1])  // it's a channel 0 compare event
    {
        // clear this event
        TIMER_MEASUREMENT->EVENTS_COMPARE[1] = 0;

        // clear/restart measurement timer
        TIMER_MEASUREMENT->TASKS_CLEAR = 1;

        // clear/restart counter
        restart_pulse_counter();

        // continue measurement with next sensor
        select_next_sensor();

        hal_gpio_write(PIN_DEBUG_MEASUREMENT_INTERVAL, 1);
    }
}
