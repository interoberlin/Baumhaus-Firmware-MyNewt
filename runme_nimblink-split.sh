#!/bin/bash -e
newt build nrf51_boot
newt build nimblink
newt create-image nimblink 0.0.0
newt load nrf51_boot
newt load nimblink 
newt debug nimblink
