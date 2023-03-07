#include "DS3231.h"
#include "I2CDevice.h"
#include <ctime>
#include <iostream>
#include <unistd.h>

using namespace std;
using namespace EE513;

#define DS3231_I2C_ADDRESS 0x68
// ADDRESSES FOR DS3231

int main(void) {
  DS3231 rtc;

  // Show Temperature
  cout << "Temperature: " << rtc.getTemperature() << " C" << endl;

  // set the time to 00:00:00
  rtc.setHour24(DS3231_HOUR, 0);
  rtc.setSecMin(DS3231_MINUTE, 0);
  rtc.setSecMin(DS3231_SECOND, 0);

  // get system time, convert to hours, minutes, seconds
  time_t now = time(0);
  tm *ltm = localtime(&now);
	// Print time
	cout << "System time: ";
	rtc.print_tm(ltm);
	// set time
	rtc.set(*ltm);
	tm *rtcTime = new tm();
	rtc.get(rtcTime);
	cout << "RTC time: ";
	rtc.print_tm(rtcTime);

	rtc.printAlarm1();
	// set alarm and test interrupt signal
	rtc.resetAlarm1Status();
	rtc.setAlarm1(EE513::AlarmType_Second,	ltm->tm_sec, ((ltm->tm_min)+2)%60, ltm->tm_hour, ltm->tm_mday);
	// set INTCN bit (bit2) to enable interrupt
	rtc.setBit(DS3231_CONTROL, 2, true);
	rtc.printAlarm1();
	
	cout << "Waiting for alarm..." << endl;
	while(true){
		unsigned char status = rtc.readRegister(DS3231_CONTROL_STATUS);

		if(status & 0x01){
			cout << "Alarm triggered!" << endl;
			// add code here to drive an LED or perform other actions on alarm
		}
		sleep(1);
	}
  //// read and display current time and date
  //while (1) {
  //  cout << "Current time: ";
  //  cout << (int)(rtc.getHour24(DS3231_HOUR)) << ":";
  //  cout << (int)(rtc.getSecMin(DS3231_MINUTE)) << ":";
  //  cout << (int)(rtc.getSecMin(DS3231_SECOND)) << endl;
  //  sleep(1);
  //}



  //// set alarm and test interrupt signal
  // rtc.writeRegister(0x0d, 0x0a); // set alarm 1 to trigger when seconds match
  // rtc.writeRegister(0x07, 0x80); // enable alarm 1 interrupt
  // cout << "Waiting for alarm..." << endl;
  // while(true){
  //	unsigned char status = rtc.readRegister(0x0f);
  //	if(status & 0x01){
  //		cout << "Alarm triggered!" << endl;
  //		// add code here to drive an LED or perform other actions on
  // alarm 		break;
  //	}
  // }

  return 0;
}
