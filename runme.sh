#!/bin/bash -e

target=$*

# newt build nrf51_boot
newt build "${target}"
newt create-image "${target}" 0.0.0
# newt load nrf51_boot
newt load "${target}" 
# newt run minitree 
