#include "I2CDevice.h"
#include <iostream>

using namespace std;
using namespace EE513;

#define DS3231_I2C_ADDRESS 0x68

int a=0;

// Set the time on the DS3231 RTC
void setTime(I2CDevice rtc, int seconds, int minutes, int hours, int day, int date, int month, int year){
	rtc.writeRegister(0, seconds);
	rtc.writeRegister(1, minutes);
	rtc.writeRegister(2, hours);
	rtc.writeRegister(3, day);
	rtc.writeRegister(4, date);
	rtc.writeRegister(5, month);
	rtc.writeRegister(6, year);
}

int main(void){
	I2CDevice rtc(1, DS3231_I2C_ADDRESS);
	rtc.writeRegister(0x0e, 0); // clear the alarm flags
	// read and display current time and date
	unsigned char* time = rtc.readRegisters(7);
	cout << "Current time: ";
	cout << (int)(time[2] & 0x1f) << ":"; // 0x1f in binary is 0001 1111
	cout << (int)(time[1] & 0x3f) << ":"; // 0x3f in binary is 0011 1111
	cout << (int)(time[0] & 0x3f) << endl;
	cout << "Current date: ";
	cout << (int)(time[4] & 0x3f) << "/";
	cout << (int)(time[5] & 0x1f) << "/";
	cout << (int)(time[6] & 0xff) << endl;

	// read and display temperature
	unsigned char tempMSB = rtc.readRegister(0x11);
	unsigned char tempLSB = rtc.readRegister(0x12);
	float temperature = (float)(((int)tempMSB << 8) | (int)tempLSB) / 256.0;
	cout << "Temperature: " << temperature << " degrees Celsius" << endl;

	// set time and date
	rtc.writeRegister(0, 0x00);
	rtc.writeRegister(1, 0x00);
	rtc.writeRegister(2, 0x00);
	rtc.writeRegister(3, 0x01);
	rtc.writeRegister(4, 0x01);
	rtc.writeRegister(5, 0x21);
	rtc.writeRegister(6, 0x22);

	// read and display new time and date
	time = rtc.readRegisters(7);
	cout << "New time: ";
	cout << (int)(time[2] & 0x1f) << ":";
	cout << (int)(time[1] & 0x3f) << ":";
	cout << (int)(time[0] & 0x3f) << endl;
	cout << "New date: ";
	cout << (int)(time[4] & 0x3f) << "/";
	cout << (int)(time[5] & 0x1f) << "/";
	cout << (int)(time[6] & 0xff) << endl;

	// set alarm and test interrupt signal
	rtc.writeRegister(0x0d, 0x0a); // set alarm 1 to trigger when seconds match
	rtc.writeRegister(0x07, 0x80); // enable alarm 1 interrupt
	cout << "Waiting for alarm..." << endl;
	while(true){
		unsigned char status = rtc.readRegister(0x0f);
		if(status & 0x01){
			cout << "Alarm triggered!" << endl;
			// add code here to drive an LED or perform other actions on alarm
			break;
		}
	}

	return 0;
}
