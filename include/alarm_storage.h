#ifndef ALARM_STORAGE_H
#define ALARM_STORAGE_H

#include "alarm.h"

void saveAlarm(const Alarm& alarm, int index = 0);
void loadAlarm(Alarm& alarm, int index = 0);
void initAlarmStorage();

#endif