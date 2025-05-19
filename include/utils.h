#ifndef UTILS_H
#define UTILS_H

#include "alarm.h"

int getCurrentYear();
int getMaxDay(int year, int month);
bool isTimeAvailable();
void setAlarmToCurrentTime(Alarm &a);

#endif
