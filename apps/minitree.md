# Minitree App

Ausgehend von der blinky app erstellen wir uns ein template für den Minitree, um die bisherige Firmware unter mynewt zu compilieren

```shell
newt target copy my_blinky minitree
Target successfully copied; targets/my_blinky --> targets/minitree

$ newt target show
targets/minitree
    app=apps/blinky
    bsp=@apache-mynewt-core/hw/bsp/nrf51dk-16kbram-openocd
    build_profile=debug
    cflags=-Wno-format-truncation 
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

$ newt target set minitree app=apps/minitree
Target targets/minitree successfully set target.app to apps/minitree

$ newt target show
targets/minitree
    app=apps/minitree
    bsp=@apache-mynewt-core/hw/bsp/nrf51dk-16kbram-openocd
    build_profile=debug
    cflags=-Wno-format-truncation 
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
```

```shell
$ cd apps

$ cp -r blinky minitree
```

### Minitree Tasksteuerung

main.c wird durch das Tasksample von [mynewt](https://mynewt.apache.org/latest/os/core_os/task/task/) ersetzt und `#define ANDRU_BLINK_PIN 28` auf unseren GPIO-Pin gesetzt.
__