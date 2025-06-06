#pragma once

#include "alarm.h"

int getCurrentYear();
int getMaxDay(int year, int month);
bool isTimeAvailable();
void setAlarmToCurrentTime(Alarm &a);
String getFormattedTime();
String getFormattedDate();
bool isFieldVisible(AlarmType type, AlarmField field);

bool connectWifi();
void initNTP();
int adjustVisibleStart(int selectedIndex, int visibleStart, int maxVisible, int totalItems);

extern unsigned long lastInteraction;
void recordInteraction();
const char* getAlarmTypeLabel(AlarmType type);
const char* getMelodyName(int melodyIndex);


// utils.h
String getRepeatDaysString(bool repeatDays[7]);
