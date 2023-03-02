#!/bin/bash
#
# Must be run as root
if [ "$EUID" -ne 0 ]
	then echo "Please run as root"
	exit
fi
# Get the current system time

function 

# Set the DS3231 real-time clock time to the current system time
# Seconds
i2cset -y 1 0x68 0x00 $((0x$(echo $SYS_TIME_HEX | cut -c15-16)))
# Minutes
i2cset -y 1 0x68 0x01 $((0x$(echo $SYS_TIME_HEX | cut -c13-14)))
# Hours
i2cset -y 1 0x68 0x02 $((0x$(echo $SYS_TIME_HEX | cut -c11-12)))
# Day of the week
i2cset -y 1 0x68 0x03 $((0x$(echo $SYS_TIME_HEX | cut -c9-10)))
# Date
i2cset -y 1 0x68 0x04 $((0x$(echo $SYS_TIME_HEX | cut -c7-8)))
# Month / Century
i2cset -y 1 0x68 0x05 $((0x$(echo $SYS_TIME_HEX | cut -c5-6)))
# Year
i2cset -y 1 0x68 0x06 $((0x$(echo $SYS_TIME_HEX | cut -c3-4)))

echo "Set time of DS3231 to system time: $(date)"
