#include <time.h>
#include "utils.h"
#include "config.h"

int getCurrentYear() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    return timeinfo.tm_year + 1900;
  }
  return 2025;
}

int getMaxDay(int year, int month) {
  struct tm t = {0};
  t.tm_year = year - 1900;
  t.tm_mon = month;
  t.tm_mday = 0;
  mktime(&t);
  return t.tm_mday;
}

bool isTimeAvailable() {
  struct tm t;
  return getLocalTime(&t) && (t.tm_year + 1900 >= 2024);
}

void setAlarmToCurrentTime(Alarm &a) {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    a.hour = 7;
    a.minute = 0;
    a.year = timeinfo.tm_year + 1900;
    a.month = timeinfo.tm_mon + 1;
    a.day = timeinfo.tm_mday;
  } else {
    a.hour = 9;
    a.minute = 9;
    a.year = 2020;
    a.month = 9;
    a.day = 9;
  }
}
