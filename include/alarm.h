#ifndef ALARM_H
#define ALARM_H

#include <Arduino.h>

enum AlarmType {
  ONE_TIME,
  SPECIFIC_DATE,
  REPEATED
};

enum AlarmField {
  ALARM_TYPE,
  ALARM_TIME_HOUR,
  ALARM_TIME_MIN,
  ALARM_DATE_YEAR,
  ALARM_DATE_MONTH,
  ALARM_DATE_DAY,
  ALARM_REPEAT_DAYS,
  ALARM_ENABLED,
  ALARM_MELODY,
  ALARM_SAVE_EXIT
};

struct Alarm {
  bool enabled;
  AlarmType type;
  int hour, minute, year, month, day;
  bool repeatDays[7];
  int melody;
  String name;
};

extern Alarm alarms[3];
extern int lastTriggerMinute;
extern bool alarmActive;

bool isFieldVisible(AlarmType type, AlarmField field);
void checkAndTriggerAlarms();

#endif
