#!/bin/bash -e
newt build nrf51_boot
newt build minitree
newt create-image minitree 0.0.0
newt load nrf51_boot
newt load minitree 
newt run minitree 
