#!/bin/bash

hex=$(i2cget -y 1 0x68 $1)
hex=${hex:2} # Remove 0x

# Convert hex to binary
bin=$(echo "obase=2; ibase=16; $hex" | bc)
# Show leading zeros
bin=$(printf "%08d" $bin)

echo $bin
