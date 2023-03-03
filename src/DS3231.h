#ifndef DS3231_H
#define DS3231_H

#include "I2CDevice.h"
#include <iostream>

namespace EE513 {

/**
 * @class DS3231
 * @brief Class that represents the DS3231 RTC module
 */
class DS3231 : public I2CDevice {
public:
  DS3231();
  virtual ~DS3231();

  uint8_t getSecond();
  void setSecond(uint8_t second);
  uint8_t getMinute();
  void setMinute(uint8_t minute);
  void set12HourMode(unsigned int reg, bool mode);
  bool is12HourMode();
  uint8_t getHour(unsigned int reg, bool mode);
  void setHour(unsigned int reg, bool mode, uint8_t hour);

private:
  uint8_t parseSecMin(uint8_t data);

  //// 读取并显示当前 RTC 模块时间和日期
  //// Read and display the current time and date from the RTC module
  // void displayTimeDate();

  //// 读取并显示当前温度
  //// Read and display the current temperature
  // float readTemperature();

  //// 在 RTC 模块上设置当前时间和日期
  //// Set the current time and date on the RTC module
  // void setTimeDate(int year, int month, int day, int hour, int minute,
  //                  int second);

  //// 设置和读取两个闹钟
  //// Set and read two alarms
  // void setAlarm1(int alarmNum, int alarmType, int year, int month, int day,
  //                int hour, int minute, int second);
  // void getAlarm1(int alarmNum, int &alarmType, int &year, int &month, int
  // &day,
  //                int &hour, int &minute, int &second);
  // void setAlarm2(int alarmNum, int alarmType, int year, int month, int day,
  //                int hour, int minute, int second);
  // void getAlarm2(int alarmNum, int &alarmType, int &year, int &month, int
  // &day,
  //                int &hour, int &minute, int &second);

  //// 评估闹钟状态
  //// Evaluate the alarm status
  // bool evaluateAlarm(int alarmNum);

private:
  // 辅助函数，将二进制转换为BCD码
  // Helper function to convert binary coded decimal to normal decimal numbers
  uint8_t dec2Bcd(uint8_t val);
  // 辅助函数，将BCD码转换为二进制
  // Helper function to convert decimal to binary coded decimal
  uint8_t bcd2Dec(uint8_t val);
};

} // namespace EE513

#endif
