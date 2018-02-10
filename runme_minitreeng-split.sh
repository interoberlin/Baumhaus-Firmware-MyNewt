#!/bin/bash -e
newt build nrf51_boot
echo "###################################"
newt build minitreeng
echo "###################################"
newt create-image minitreeng 0.0.0
echo "###################################"
newt load nrf51_boot
echo "###################################"
newt load minitreeng 
echo "###################################"
#newt run minitreeng 0
