#ifndef DS3231_H
#define DS3231_H

#include "I2CDevice.h"
#include <cstdint>
#include <iostream>

#define DS3231_SECOND 0x00
#define DS3231_MINUTE 0x01
#define DS3231_HOUR 0x02
#define DS3231_DAY 0x03
#define DS3231_DATE 0x04
#define DS3231_MONTH_CENTURY 0x05
#define DS3231_YEAR 0x06
#define DS3231_ALARM1_SECOND 0x07
#define DS3231_ALARM1_MINUTE 0x08
#define DS3231_ALARM1_HOUR 0x09
#define DS3231_ALARM1_DAY_DATE 0x0a
#define DS3231_ALARM2_MINUTE 0x0b
#define DS3231_ALARM2_HOUR 0x0c
#define DS3231_ALARM2_DAY_DATE 0x0d
#define DS3231_CONTROL 0x0e
#define DS3231_CONTROL_STATUS 0x0f
#define DS3231_AGING_OFFSET 0x10
#define DS3231_TEMP_MSB 0x11
#define DS3231_TEMP_LSB 0x12

namespace EE513 {

  enum AlarmType {
    AlarmType_OncePerSecond = 0x0f,        // 1111b 0x0f
    AlarmType_Second = 0x0e,               // 1110b 0x0e
    AlarmType_MinuteSecond = 0x0c,         // 1100b 0x0c
    AlarmType_HourMinuteSecond = 0x08,     // 1000b 0x08
    AlarmType_DateHourMinuteSecond = 0x00, // 0000b 0x00
    AlarmType_DayHourMinuteSecond = 0x10   // 10000b 0x10
  };

/**
 * @class DS3231
 * @brief Class that represents the DS3231 RTC module
 */
class DS3231 : public I2CDevice {
public:
  DS3231();
  virtual ~DS3231();

	void setBit(uint8_t reg, uint8_t bit, bool value);
  uint8_t getSecMin(uint8_t reg);
  void setSecMin(uint8_t reg, uint8_t second);
  void set12HourMode(uint8_t reg, bool mode);
  bool is12HourMode(uint8_t reg);
  uint8_t getHour24(uint8_t reg);
  void setHour24(uint8_t reg, uint8_t hour);
  uint8_t getDayOfWeek(uint8_t reg);
  void setDayOfWeek(uint8_t reg, uint8_t day);
  uint8_t getDate(uint8_t reg);
  void setDate(uint8_t reg, uint8_t date);
  uint8_t getMonth(uint8_t reg);
  void setMonth(uint8_t reg, uint8_t month);
  uint32_t getMainYear();
  void setMainYeay(uint32_t year);
  float getTemperature();

  void set(struct tm time);
  void get(struct tm *time);

  void setAlarm1(AlarmType type, struct tm time);
  void setAlarm2(AlarmType type, struct tm time);
	void getAlarm1(AlarmType &type, struct tm *time);
	void getAlarm2(AlarmType &type, struct tm *time);
	bool hasAlarm1();
	bool hasAlarm2();
	void clearAlarm1();
	void clearAlarm2();
	void printAlarm1();
	void printAlarm2();

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
};

} // namespace EE513

#endif
