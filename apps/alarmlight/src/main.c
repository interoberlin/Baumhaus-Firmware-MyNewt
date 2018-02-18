/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 * 
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#include <assert.h>
#include <string.h>

#include <sysinit/sysinit.h>
#include <reboot/log_reboot.h>
#include <config/config.h>
#include <os/os.h>
#include <bootutil/image.h>
#include <bootutil/bootutil.h>
#include <bsp/bsp.h>
#include <hal/hal_gpio.h>

#if MYNEWT_VAL(SPLIT_LOADER)
#include <split/split.h>
#endif

#include "alarmlight.h"

#ifdef ARCH_sim
#include "mcu/mcu_sim.h"
#endif

static volatile int g_task1_loops;

/* For LED toggling */
int g_led_pin;

/**
 * main
 *
 * The main task for the project. This function initializes packages,
 * and then blinks the BSP LED in a loop.
 *
 * @return int NOTE: this function should never return!
 */
int
main(int argc, char **argv)
{
    int rc;

#ifdef ARCH_sim
    mcu_sim_parse_args(argc, argv);
#endif

    sysinit();

    conf_load();
    
    reboot_start(hal_reset_cause());

    // fuer unser development board anpassen
    hal_gpio_init_out(NRFDUINO_PIN_LED, 1);
    hal_gpio_init_out(NRFDUINO_CTRL_LED, 1);

    while (1) {
        ++g_task1_loops;

        /* Wait one second */
        os_time_delay(OS_TICKS_PER_SEC);
		
        /* Toggle the LED */
        hal_gpio_toggle(NRFDUINO_PIN_LED);
        hal_gpio_toggle(NRFDUINO_CTRL_LED);
        
    }
    assert(0);

    return rc;
}
