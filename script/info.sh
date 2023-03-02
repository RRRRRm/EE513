#!/bin/bash

# Must be run as root
if [ "$EUID" -ne 0 ]
	then echo "Please run as root"
	exit 1
fi
# Get the time from the DS3231 real-time clock
REG_SECONDS=$(i2cget -y 1 0x68 0x00)
REG_MINUTES=$(i2cget -y 1 0x68 0x01)
REG_HOURS=$(i2cget -y 1 0x68 0x02)
REG_DAY=$(i2cget -y 1 0x68 0x03)
REG_DATE=$(i2cget -y 1 0x68 0x04)
REG_MONTH=$(i2cget -y 1 0x68 0x05)
REG_YEAR=$(i2cget -y 1 0x68 0x06)

# Convert the values to decimal
SECONDS=$(printf "%d" $REG_SECONDS)
MINUTES=$(printf "%d" $REG_MINUTES)
HOURS=$(printf "%d" $REG_HOURS)
DAY=$(printf "%d" $REG_DAY)
DATE=$(printf "%d" $REG_DATE)
MONTH=$(printf "%d" $REG_MONTH)
YEAR=$(printf "%d" $REG_YEAR)

# Convert the 12-hour time to 24-hour time if necessary
if ((($HOURS & 0x40) == 0x40)); then
	# 12-hour mode with PM indicator
	if ((($HOURS & 0x20) == 0x20)); then
		HOURS=$((($HOURS & 0x1F) + 12))
		# 12-hour mode with AM indicator
	else
		HOURS=$((($HOURS & 0x1F)))
	fi
else
	# 24-hour mode
	HOURS=$((($HOURS & 0x3F)))
fi

# Convert the binary-coded decimal (BCD) values to decimal
DAY=$(($DAY & 0x07))
DATE=$((($DATE & 0x0F) + ($DATE >> 4) * 10))
MONTH=$((($MONTH & 0x0F) + ($MONTH >> 4) * 10))
YEAR=$((($YEAR & 0x0F) + ($YEAR >> 4) * 10))

# Print out the time and date information
echo "Current time: $HOURS:$MINUTES:$SECONDS"
echo "Current date: $(date +%A), $(date +%B) $DATE, 20$YEAR"

# Print out the status of the DS3231 clock
STATUS_REG=$(i2cget -y 1 0x68 0x0F)
if ((($STATUS_REG & 0x20) == 0x20)); then
	echo "Oscillator stopped"
else
	echo "Oscillator running"
fi

# Print out the alarm information
ALARM_REG=$(i2cget -y 1 0x68 0x0E)
if ((($ALARM_REG & 0x01) == 0x01)); then
	echo "Alarm 1 enabled"
else
	echo "Alarm 1 disabled"
fi
if ((($ALARM_REG & 0x02) == 0x02)); then
	echo "Alarm 2 enabled"
else
	echo "Alarm 2 disabled"
fi

# Print out the temperature information
TEMP_MSB=$(i2cget -y 1 0x68 0x11)
TEMP_LSB=$(i2cget -y 1 0x68 0x12)

# Convert the temperature values to decimal

TEMP_MSB_DEC=$(($TEMP_MSB & 0x7F))
TEMP_LSB_DEC=$((($TEMP_LSB >> 6) * 25))

# Check if the temperature is negative

if ((($TEMP_MSB & 0x80) == 0x80)); then
	TEMPERATURE=$((($TEMP_MSB_DEC * -1) - $TEMP_LSB_DEC))
else
	TEMPERATURE=$((($TEMP_MSB_DEC) + $TEMP_LSB_DEC))
fi

echo "Temperature: $TEMPERATURE °C"

# Print out the control register information

CONTROL_REG=$(i2cget -y 1 0x68 0x0E)
if ((($CONTROL_REG & 0x04) == 0x04)); then
	echo "Square-wave output enabled"
else
	echo "Square-wave output disabled"
fi

#Print out the aging offset information

AGING_OFFSET=$(i2cget -y 1 0x68 0x10)
if ((($AGING_OFFSET & 0x80) == 0x80)); then
	AGING_OFFSET=$((($AGING_OFFSET & 0x7F) - 128))
else
	AGING_OFFSET=$((($AGING_OFFSET & 0x7F)))
fi
echo "Aging offset: $AGING_OFFSET ppm"
