#include "DS3231.h"

#include <cstdint>
#include <cstring>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sstream>
#include <stdexcept>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>

#define DS3231_ADDR 0x68
#define I2C_BUS 1
// Convert BCD to Binary, 15 = 00001111
#define BCD_TO_BIN(val) ((val) = ((val)&15) + ((val) >> 4) * 10)
// Convert Binary to BCD
#define BIN_TO_BCD(val) ((val) = ((val) / 10) << 4 | ((val) % 10))

namespace EE513 {

// init the base class
DS3231::DS3231() : I2CDevice(I2C_BUS, DS3231_ADDR) {}
DS3231::~DS3231() {}


void DS3231::setBit(uint8_t reg, uint8_t bit, bool value) {
	uint8_t data = this->readRegister(reg);
	if (value) {
		data |= (1 << bit);
	} else {
		data &= ~(1 << bit);
	}
	this->writeRegister(reg, data);
}

/**
 * Get the Second or Minute from the DS3231 rtc
 * @param reg The register to get the minute from (DS3231_MINUTE or
 * DS3231_ALARM1_MINUTE or DS3231_ALARM2_MINUTE or DS3231_SECOND or
 * DS3231_ALARM1_SECOND)
 * @return The minute
 */
uint8_t DS3231::getSecMin(uint8_t reg) {
  uint8_t data = this->readRegister(reg);
  data &= 0x7f; // clear the 8th bit
  return BCD_TO_BIN(data);
}

/**
 * Set the minute of the DS3231 rtc
 * @param reg The register to set the minute from (DS3231_MINUTE or
 * DS3231_ALARM1_MINUTE or DS3231_ALARM2_MINUTE or DS3231_SECOND or
 * DS3231_ALARM1_SECOND)
 * @param min The minute to set
 */
void DS3231::setSecMin(uint8_t reg, uint8_t number) {
  if (number > 59) {
    throw std::invalid_argument("Second or minute must be between 0 and 59");
  }
  uint8_t data = this->readRegister(reg);
  data &= 0x80; // Keep the 8th bit
  // set the lower 7 bits
  data |= BIN_TO_BCD(number);
  this->writeRegister(DS3231_SECOND, data);
}

// The hour of the DS3231 rtc
// bit 6 is 0 for 24 hour mode, 1 for 12 hour mode
// 12 hour format: bit 5 is 0 for AM, 1 for pm
// 24 hour format: bit 5 is 20 Hour
// bit 4 is ten Hour
// bits 3-0 are the hour

/**
 * Set the 12 hour mode
 * @param reg The register to set the mode for (DS3231_HOUR or
 * DS3231_ALARM1_HOUR or DS3231_ALARM2_HOUR)
 * @param mode12 0 = 12 hour mode, 1 = 24 hour mode
 */
void DS3231::set12HourMode(uint8_t reg, bool mode12) {
  uint8_t data = this->readRegister(reg);
  if (mode12) {
    data |= 0x40; // 0x40 in binary is 0100 0000
  } else {
    data &= 0xBF; // 0xBF in binary is 1011 1111
  }
}

/**
 * @param reg The register to get the mode for (DS3231_HOUR or
 * DS3231_ALARM1_HOUR or DS3231_ALARM2_HOUR)
 * @return 0 = 12 hour mode, 1 = 24 hour mode
 */
bool DS3231::is12HourMode(uint8_t reg) {
  uint8_t data = this->readRegister(reg);
  return (data & 0x40) == 0x40;
}

/**
 * Get the hour from the DS3231 rtc in 24 hour format
 * @param reg The register to get the hour from (DS3231_HOUR or
 * DS3231_ALARM1_HOUR or DS3231_ALARM2_HOUR)
 * @return The hour in 24 hour format
 */
uint8_t DS3231::getHour24(uint8_t reg) {
  uint8_t data = this->readRegister(reg);
  // Get the 10 hour and the hour
  uint8_t hour = ((data & 0x1F) >> 4) * 10 + (data & 0x0F);
  // 0110 0000 in binary is 0x60
  switch (data & 0x60) {
  case 0x20:
    // 0x20 in binary is 0[01]0 0000
    // 24 hour format, 20 hour bit is set
    hour += 20;
    break;
  case 0x60:
    // 0x60 in binary is 0[11]0 0000
    // 12 hour format, PM bit is set
    hour += 12;
    break;
  }
  return hour;
}

/**
 * Set the hour on the DS3231 rtc in 24 hour format
 * @param reg The register to set the hour for (DS3231_HOUR or
 * DS3231_ALARM1_HOUR or DS3231_ALARM2_HOUR)
 * @param hour The hour in 24 hour format
 */
void DS3231::setHour24(uint8_t reg, uint8_t hour) {
  if (hour > 23) {
    throw std::invalid_argument("Hour must be between 0 and 23");
  }
  uint8_t data = this->readRegister(reg);
  // Set the 10 hour and the hour
  data &= 0x80; // 0x80 in binary is 1000 0000
  data |= BIN_TO_BCD(hour);
  this->writeRegister(reg, data);
}

/**
 * Get the hour from the DS3231 rtc in 12 hour format
 * @param reg The register to get the hour from (DS3231_HOUR or
 * DS3231_ALARM1_HOUR or DS3231_ALARM2_HOUR)
 * @return The hour in 12 hour format
 */
uint8_t DS3231::getDayOfWeek(uint8_t reg) {
  uint8_t data = this->readRegister(reg);
  data &= 0x07; // 0x07 in binary is 0000 0111
  return BCD_TO_BIN(data);
}

/**
 * Set the hour on the DS3231 rtc in 12 hour format
 * @param reg The register to set the hour for (DS3231_HOUR or
 * DS3231_ALARM1_HOUR or DS3231_ALARM2_HOUR)
 * @param hour The hour in 12 hour format
 */

void DS3231::setDayOfWeek(uint8_t reg, uint8_t day) {
  if (day > 7) {
    throw std::invalid_argument("Day must be between 1 and 7");
  }
  uint8_t data = this->readRegister(reg);
  data &= 0xF8; // 0xF8 in binary is 1111 1000
  data |= BIN_TO_BCD(day);
  this->writeRegister(reg, data);
}

void DS3231::setDate(uint8_t reg, uint8_t date) {
  if (date > 31) {
    throw std::invalid_argument("Date must be between 1 and 31");
  }
  uint8_t data = this->readRegister(reg);
  data &= 0xC0; // 0xC0 in binary is 1100 0000
  data |= BIN_TO_BCD(date);
  this->writeRegister(reg, data);
}

uint8_t DS3231::getDate(uint8_t reg) {
  uint8_t data = this->readRegister(reg);
  data &= 0x3F; // 0x3F in binary is 0011 1111
  return BCD_TO_BIN(data);
}

void DS3231::setMonth(uint8_t reg, uint8_t month) {
  if (month > 12) {
    throw std::invalid_argument("Month must be between 1 and 12");
  }
  uint8_t data = this->readRegister(reg);
  data &= 0xE0; // 0xE0 in binary is 1110 0000 (clear the month bits)
  data |= BIN_TO_BCD(month);
  this->writeRegister(reg, data);
}

/**
 * Get the month from the DS3231 rtc
 * @param reg The register to get the month from (DS3231_MONTH_CENTURY or
 * DS3231_ALARM1_MONTH or DS3231_ALARM2_MONTH)
 * @return The month
 */
uint8_t DS3231::getMonth(uint8_t reg) {
  uint8_t data = this->readRegister(reg);
  data &= 0x1F; // 0x1F in binary is 0001 1111 (clear the century bit)
  return BCD_TO_BIN(data);
}

/**
 * Set the year on the DS3231 rtc
 * @param year The year
 */
void DS3231::setMainYeay(uint32_t year) {
  if (year > 2099 || year < 1900)
    throw std::invalid_argument("Year must be between 1900 and 2099, but was " +
																std::to_string(year));
  uint8_t dataC = this->readRegister(DS3231_MONTH_CENTURY);
  // Century bit 1 means 21th century, Century bit 0 means 20th Century
  dataC = (year > 1999) ? (dataC | 0x80)  // 0x80 in binary is 1000 0000
                        : (dataC & 0x7F); // 0x7F in binary is 0111 1111
  this->writeRegister(DS3231_MONTH_CENTURY, dataC);
  year %= 100;
  this->writeRegister(DS3231_YEAR, BIN_TO_BCD(year));
}

/**
 * Get the year from the DS3231 rtc
 * @return The year
 */
uint32_t DS3231::getMainYear() {
  // Century bit 1 means 21th century
  // Century bit 0 means 20th Century
  // https://github.com/zephyrproject-rtos/zephyr/issues/40810
  uint8_t dataY = this->readRegister(DS3231_YEAR);
  uint32_t year = BCD_TO_BIN(dataY) + 1900;
  if (this->readRegister(DS3231_MONTH_CENTURY) & 0x80) {
    year += 100;
  }
  return year;
}

float DS3231::getTemperature() {
  int8_t tempInt = this->readRegister(DS3231_TEMP_MSB);
  float temp = ((this->readRegister(DS3231_TEMP_LSB)) >> 6) * 0.25;
  temp += (float)tempInt;
  return temp;
}

void DS3231::set(struct tm time) {
  this->setSecMin(DS3231_SECOND, time.tm_sec);
  this->setSecMin(DS3231_MINUTE, time.tm_min);
  this->setHour24(DS3231_HOUR, time.tm_hour);
  this->setDayOfWeek(DS3231_DAY, time.tm_wday);
  this->setDate(DS3231_DATE, time.tm_mday);
  this->setMonth(DS3231_MONTH_CENTURY, time.tm_mon);
  this->setMainYeay(time.tm_year+1900);
}

void DS3231::get(struct tm *time) {
  time->tm_sec = this->getSecMin(DS3231_SECOND);
  time->tm_min = this->getSecMin(DS3231_MINUTE);
  time->tm_hour = this->getHour24(DS3231_HOUR);
  time->tm_wday = this->getDayOfWeek(DS3231_DAY);
  time->tm_mday = this->getDate(DS3231_DATE);
  time->tm_mon = this->getMonth(DS3231_MONTH_CENTURY);
  time->tm_year = this->getMainYear()-1900;
}

void DS3231::setAlarm1(AlarmType type, struct tm time) {
	// Set the alarm type bits to first bit of DS3231_ALARM1_SECOND, DS3231_ALARM1_MINUTE, DS3231_ALARM1_HOUR, DS3231_ALARM1_DAY
	unsigned int typeMast = (unsigned int)type;

	uint8_t data = this->readRegister(DS3231_ALARM1_SECOND);
	data &= 0x80; // 0x80 in binary is 1000 0000
	data |= (type << 4);
	this->writeRegister(DS3231_ALARM1_SECOND, data);
	this->setSecMin(DS3231_ALARM1_SECOND, time.tm_sec);
	this->setSecMin(DS3231_ALARM1_MINUTE, time.tm_min);
	this->setHour24(DS3231_ALARM1_HOUR, time.tm_hour);
	this->setDayOfWeek(DS3231_ALARM1_DAY, time.tm_wday);
}
}

} // namespace EE513
