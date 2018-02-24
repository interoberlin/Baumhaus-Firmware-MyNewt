#!/bin/bash -e

target=$*

#newt -s build nrf51_boot
newt -s build "${target}"
newt -s create-image "${target}" 0.0.0
# newt load nrf51_boot
newt load "${target}" 
# newt run minitree 
