#!/bin/bash

hex=$(i2cget -y 1 0x68 $1)

# Convert hex to dec
dec=$(printf '%d' "$hex")

# Convert dec to binary
bin=$(echo "obase=2; $dec" | bc)
# Show leading zeros
bin=$(printf "%08d" $bin)

echo $bin $dec $hex
