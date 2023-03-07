#include "DS3231.h"
#include <iostream>
#include <string>
#include <unistd.h>
#include <vector>

using namespace std;
using namespace EE513;

// This file provides a simple command line interface to the DS3231 RTC
// module. It allows the user to read and write the time and date, set alarms
// and read the temperature.

void printHelp() {
  cout << "Usage: alarm [OPTION]...\n"
       << "A command line tool to set alarms.\n\n"
       << "Options:\n"
       << "  --set-time    Set the system time.\n"
       << "  --set-alarm1  Set the first alarm time.\n"
       << "  --set-alarm2  Set the second alarm time.\n"
       << "  --get-time    Get the current system time.\n"
       << "  --get-alarm1  Get the first alarm time.\n"
       << "  --get-alarm2  Get the second alarm time.\n"
       << "  --help        Display this help and exit.\n"
       << endl;
}

int main(int argc, char *argv[]) {
  DS3231 rtc;
  // Parse command line arguments
  vector<string> args(argv + 1, argv + argc);
  if (args.size() == 0 || args[0] == "--help") {
    printHelp();
  } else if (args[0] == "--set-time" && args.size() == 2) {

  } else if (args[0] == "--set-alarm1" && args.size() == 2) {

  } else if (args[0] == "--set-alarm2" && args.size() == 2) {
  } else if (args[0] == "--get-time" && args.size() == 1) {
  } else if (args[0] == "--get-alarm1" && args.size() == 1) {
  } else if (args[0] == "--get-alarm2" && args.size() == 1) {
  } else {
    cerr << "Invalid command" << endl;
    printHelp();
    return 1;
  }
  return 0;
}

void pu
