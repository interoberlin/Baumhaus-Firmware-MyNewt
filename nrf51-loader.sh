#!/bin/bash -e
newt build nrf51_boot
newt load nrf51_boot
newt build nrf51-loader
newt create-image nrf51-loader 0.0.0
newt load nrf51-loader