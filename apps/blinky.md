# Blinky App

Mynewt installiert zuerst die App my_blinky_sim.
Ausgehend von dieser wird ein Bootloader und ein App-Image erstellt:

```shell
$ newt target show
targets/my_blinky
    app=apps/blinky
    bsp=@apache-mynewt-core/hw/bsp/nrf51dk-16kbram-openocd
    build_profile=debug
```

## targets/blinky/src.main.c

Hier muss der zu schaltende GPIO-Pin auf 28 gesetzt werden. YMMV.

```c
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

#include "sysinit/sysinit.h"
#include "os/os.h"
#include "bsp/bsp.h"
#include "hal/hal_gpio.h"
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

    // Anpassung fuer unser development board
    g_led_pin = 28;
    hal_gpio_init_out(g_led_pin, 1);

    while (1) {
        ++g_task1_loops;

        /* Wait one second */
        os_time_delay(OS_TICKS_PER_SEC);

        /* Toggle the LED */
        hal_gpio_toggle(g_led_pin);
    }
    assert(0);

    return rc;
}
```

## Ein App-Image für ein nrf51-Board erstellen

```shell
$ newt target copy my_blinky_sim my_blinky
Target successfully copied; targets/my_blinky_sim --> targets/my_blinky

$ newt target show
targets/my_blinky
    app=apps/blinky
    bsp=@apache-mynewt-core/hw/bsp/native
    build_profile=debug
targets/my_blinky_sim
    app=apps/blinky
    bsp=@apache-mynewt-core/hw/bsp/native
    build_profile=debug
```

Für das target my_blinky muss nun noch der bsp konfiguriert werden damit es auf für die Zielplattform übersetzt wird:

```shell
$ newt target set my_blinky bsp=@apache-mynewt-core/hw/bsp/nrf51dk_16kbram_openocd
Target targets/my_blinky successfully set target.bsp to @apache-mynewt-core/hw/bsp/nrf51dk_16kbram_openocd

$ newt target show
targets/my_blinky
    app=apps/blinky
    bsp=@apache-mynewt-core/hw/bsp/nrf51dk_16kbram_openocd
    build_profile=debug
targets/my_blinky_sim
    app=apps/blinky
    bsp=@apache-mynewt-core/hw/bsp/native
    build_profile=debug
```

Ein erster Buildversuch schlägt fehl:

```shell
$ newt build my_blinky
Building target targets/my_blinky
Assembling repos/apache-mynewt-core/hw/bsp/nrf51dk_16kbram_openocd/src/arch/cortex_m0/gcc_startup_nrf51.s

[...]

Compiling repos/apache-mynewt-core/hw/bsp/nrf51dk_16kbram_openocd/src/sbrk.c
Error: repos/apache-mynewt-core/libc/baselibc/src/sprintf.c: In function 'sprintf':
repos/apache-mynewt-core/libc/baselibc/src/sprintf.c:14:5: error: specified bound 4294967295 exceeds maximum object size 2147483647 [-Werror=format-truncation=]
  rv = vsnprintf(buffer, ~(size_t) 0, format, ap);
  ~~~^~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
cc1: all warnings being treated as errors
```

Per default werden alle Compile-Warnings als Error gewertet. Einzelne Warnungen kann man mit einem Flag unterdrücken. Man kann die Compilerflags für den Buldprozess pro App setzen. 

```shell
$ newt target set my_blinky cflags=-Wno-format-truncation  
Target targets/my_blinky successfully set pkg.cflags to -Wno-format-truncation
```

Das App-Image lässt sich nun bauen.

```shell
$ newt build my_blinky
Building target targets/my_blinky
Compiling repos/apache-mynewt-core/libc/baselibc/src/sprintf.c

[...]

Linking /home/gvr/src/gvr/baumhaus/newt/newt-Baumhaus-Tree-Firmware/bin/targets/my_blinky/app/apps/blinky/blinky.elf
Target successfully built: targets/my_blinky
```

## Einen Bootloader für das nrf51 erstellen

```shell
$ newt target create nrf51_boot
Target targets/nrf51_boot successfully created

$ newt target set nrf51_boot app=@apache-mynewt-core/apps/boot
Target targets/nrf51_boot successfully set target.app to @apache-mynewt-core/apps/boot

$ newt target set nrf51_boot bsp=@apache-mynewt-core/hw/bsp/nrf51dk-16kbram-openocd
Target targets/nrf51_boot successfully set target.bsp to @apache-mynewt-core/hw/bsp/nrf51dk-16kbram-openocd

$ newt target set nrf51_boot build_profile=debug
Target targets/nrf51_boot successfully set target.build_profile to debug

$ newt target set nrf51_boot cflags=-Wno-format-truncation
Target targets/nrf51_boot successfully set pkg.cflags to -Wno-format-truncation

$ newt target show 
targets/my_blinky
    app=apps/blinky
    bsp=@apache-mynewt-core/hw/bsp/nrf51dk-16kbram-openocd
    build_profile=debug
    cflags=-Wno-format-truncation 
targets/my_blinky_sim
    app=apps/blinky
    bsp=@apache-mynewt-core/hw/bsp/native
    build_profile=debug
targets/nrf51_boot
    app=@apache-mynewt-core/apps/boot
    bsp=@apache-mynewt-core/hw/bsp/nrf51dk-16kbram-openocd
    build_profile=debug
    cflags=-Wno-format-truncation 

$ newt build nrf51_boot
Building target targets/nrf51_boot
Compiling repos/apache-mynewt-core/boot/bootutil/src/image_ec.c

[...]

Linking /home/gvr/src/gvr/baumhaus/newt/newt-Baumhaus-Tree-Firmware/bin/targets/nrf51_boot/app/apps/boot/boot.elf
Target successfully built: targets/nrf51_boot
newt build nrf51_boot  24,90s user 5,29s system 309% cpu 9,764 total
```

## Das gesignte Image erstellen

```shell
$ newt create-image my_blinky 1.0.0
App image succesfully generated: /home/gvr/src/gvr/baumhaus/newt/newt-Baumhaus-Tree-Firmware/bin/targets/my_blinky/app/apps/blinky/blinky.img
```

## laden der beiden Images

```shell
$ newt load nrf51_boot
Loading bootloader
$ newt load my_blinky
Loading app image into slot 1
```

## unter GDB laufen lassen

```shell
newt run my_blinky 0.0.0
Loading app image into slot 1
[/home/gvr/src/gvr/baumhaus/newt/newt-Baumhaus-Tree-Firmware/repos/apache-mynewt-core/hw/bsp/nrf51dk-16kbram-openocd/nrf51dk-16kbram-openocd_debug.sh /home/gvr/src/gvr/baumhaus/newt/newt-Baumhaus-Tree-Firmware/repos/apache-mynewt-core/hw/bsp/nrf51dk-16kbram-openocd /home/gvr/src/gvr/baumhaus/newt/newt-Baumhaus-Tree-Firmware/bin/targets/my_blinky/app/apps/blinky/blinky]
Open On-Chip Debugger 0.10.0+dev-00143-gf6449a7cb (2017-05-27-09:57)
Licensed under GNU GPL v2
For bug reports, read
	http://openocd.org/doc/doxygen/bugs.html
Info : FTDI SWD mode enabled
cortex_m reset_config sysresetreq
adapter speed: 1000 kHz
Info : clock speed 1000 kHz
Info : SWD DPIDR 0x0bb11477
Info : nrf51.cpu: hardware has 4 breakpoints, 2 watchpoints
target halted due to debug-request, current mode: Thread 
xPSR: 0x61000000 pc: 0x0000163a msp: 0x20003fc8
GNU gdb (GDB) 7.12.1
Copyright (C) 2017 Free Software Foundation, Inc.
License GPLv3+: GNU GPL version 3 or later <http://gnu.org/licenses/gpl.html>
This is free software: you are free to change and redistribute it.
There is NO WARRANTY, to the extent permitted by law.  Type "show copying"
and "show warranty" for details.
This GDB was configured as "--host=x86_64-pc-linux-gnu --target=arm-none-eabi".
Type "show configuration" for configuration details.
For bug reporting instructions, please see:
<http://www.gnu.org/software/gdb/bugs/>.
Find the GDB manual and other documentation resources online at:
<http://www.gnu.org/software/gdb/documentation/>.
For help, type "help".
Type "apropos word" to search for commands related to "word"...
Reading symbols from /home/gvr/src/gvr/baumhaus/newt/newt-Baumhaus-Tree-Firmware/bin/targets/my_blinky/app/apps/blinky/blinky.elf...done.
Info : accepting 'gdb' connection on tcp/3333
Info : nRF51822-QFAA(build code: H0) 256kB Flash
0x0000163a in ?? ()
target halted due to debug-request, current mode: Thread 
xPSR: 0xc1000000 pc: 0x000000c0 msp: 0x20004000
target halted due to debug-request, current mode: Thread 
xPSR: 0xc1000000 pc: 0x000000c0 msp: 0x20004000
(gdb) c
Continuing.
^C
Program received signal SIGINT, Interrupt.
os_tick_idle (ticks=128) at repos/apache-mynewt-core/hw/mcu/nordic/nrf51xxx/src/hal_os_tick.c:161
161	    if (ticks > 0) {
(gdb) where
#0  os_tick_idle (ticks=128) at repos/apache-mynewt-core/hw/mcu/nordic/nrf51xxx/src/hal_os_tick.c:161
#1  0x000083a8 in os_idle_task (arg=<optimized out>) at repos/apache-mynewt-core/kernel/os/src/os.c:114
#2  0x00000000 in ?? ()
(gdb) 
```