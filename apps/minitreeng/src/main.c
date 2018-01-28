#include <assert.h>
#include <string.h>
#include "util/random.h"
#include <sysinit/sysinit.h>
#include <os/os.h>
#include <bsp/bsp.h>
#include <hal/hal_gpio.h>

/* BLE */
#include <nimble/ble.h>
#include <host/ble_hs.h>
#include <services/gap/ble_svc_gap.h>

#include "util/random.h"
#include "patterns/patterns.h"

#ifdef ARCH_sim
#include "mcu/mcu_sim.h"
#endif

#include <leds.h>


//extern neopixel_strip_t strip[];
//extern uint8_t leds_per_strip[];

#define ANDRU_BLINK_PIN 28

/* Create a simple "project" with a task that blinks a LED every second */

/* Define task stack and task object */
#define MY_TASK_PRIO        (OS_TASK_PRI_HIGHEST) 
#define MY_STACK_SIZE       (64) 
struct os_task my_task; 
os_stack_t my_task_stack[MY_STACK_SIZE]; 

/* This is the task function */
void my_task_func(void *arg) {
    os_sr_t sr;

    /* Set the led pin as an output */
    hal_gpio_init_out(ANDRU_BLINK_PIN, 1);

    /* The task is a forever loop that does not return */
    while (1) {
        /* Wait one second */ 
        os_time_delay(5);

        /* Toggle the LED */ 
        hal_gpio_toggle(ANDRU_BLINK_PIN);
        update_patterns();
        OS_ENTER_CRITICAL(sr);
        update_leds();
        OS_EXIT_CRITICAL(sr);
    }
}

/* This is the main function for the project */
int main(int argc, char **argv) 
{
  srand(0);
#ifdef ARCH_sim
    mcu_sim_parse_args(argc, argv);
#endif

    /* Perform system and package initialization */
    sysinit();

    ble_svc_gap_device_name_set("minitreeng");

    init_ledstrips();
    init_patterns();

    /* Initialize the task */
    os_task_init(&my_task, "my_task", my_task_func, NULL, MY_TASK_PRIO, 
                 OS_WAIT_FOREVER, my_task_stack, MY_STACK_SIZE);

    /*  Process events from the default event queue.  */
    while (1) {
       os_eventq_run(os_eventq_dflt_get());
    }
    /* main never returns */  
}
