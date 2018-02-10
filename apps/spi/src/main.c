
#include <assert.h>
#include <string.h>

#include "sysinit/sysinit.h"
#include "os/os.h"
#include "bsp/bsp.h"
#include "hal/hal_gpio.h"
#include "hal/hal_spi.h"

#ifdef ARCH_sim
#include "mcu/mcu_sim.h"
#endif


#define ANDRU_BLINK_PIN 28

/* Create a simple "project" with a task that blinks a LED every second */

/* Define task stack and task object */
#define MY_TASK_PRIO        (OS_TASK_PRI_HIGHEST) 
#define MY_STACK_SIZE       (64) 
struct os_task my_task; 
os_stack_t my_task_stack[MY_STACK_SIZE]; 


extern struct nrf51_hal_spi nrf51_hal_spi0;
extern struct hal_spi_settings dev1;


/* This is the task function */
void my_task_func(void *arg)
{
    os_sr_t sr;

    /* Set the led pin as an output */
    hal_gpio_init_out(ANDRU_BLINK_PIN, 1);

    /* The task is a forever loop that does not return */
    while (1)
    {
        /* Wait one second */ 
        os_time_delay(5);

        // send a byte for testing
//        hal_spi_transfer(pspi, 0xA4);

        /* Toggle the LED */ 
        hal_gpio_toggle(ANDRU_BLINK_PIN);
    }
}

void init_spi()
{
    hal_spi_init(SPI0); 
    hal_spi_config(0, &dev1);
}

/* This is the main function for the project */
int main(int argc, char **argv) 
{
#ifdef ARCH_sim
    mcu_sim_parse_args(argc, argv);
#endif

    /* Perform system and package initialization */
    sysinit();

    //init_spi();
    hal_bsp_init();

    /* Initialize the task */
    os_task_init(&my_task, "my_task", my_task_func, NULL, MY_TASK_PRIO, 
                 OS_WAIT_FOREVER, my_task_stack, MY_STACK_SIZE);

    /*  Process events from the default event queue.  */
    while (1)
    {
       os_eventq_run(os_eventq_dflt_get());
    }
    /* main never returns */  
}
