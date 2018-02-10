#!/bin/bash -e
newt build nrf51_boot
echo "###################################"
newt build nimblink
echo "###################################"
newt create-image nimblink 0.0.0
echo "###################################"
newt load nrf51_boot
echo "###################################"
newt load nimblink 
echo "###################################"
newt run nimblink 0
