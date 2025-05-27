#ifndef UTILS_H
#define UTILS_H

#include "alarm.h"

int getCurrentYear();
int getMaxDay(int year, int month);
bool isTimeAvailable();
void setAlarmToCurrentTime(Alarm &a);
String getFormattedTime();
String getFormattedDate();

bool connectWifi();
int adjustVisibleStart(int selectedIndex, int visibleStart, int maxVisible, int totalItems);

extern unsigned long lastInteraction;
void recordInteraction();

#endif
