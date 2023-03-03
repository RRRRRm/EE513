#include "DS3231.h"

#include <cstring>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sstream>
#include <stdexcept>
#include <sys/ioctl.h>
#include <unistd.h>

#define DS3231_ADDR 0x68
#define I2C_BUS 1

namespace EE513 {

// init the base class
DS3231::DS3231() : I2CDevice(I2C_BUS, DS3231_ADDR) {}
DS3231::~DS3231() {}



} // namespace EE513
