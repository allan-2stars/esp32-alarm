#ifndef GLOBALS_H
#define GLOBALS_H

#include <time.h>
#include <Adafruit_SSD1306.h>
#include "alarm.h"

// Shared system state
extern UIState uiState;
extern Alarm alarms[3];
extern Alarm tempAlarm;
extern int selectedAlarmIndex;
extern AlarmField selectedField;
extern int currentRepeatDayIndex;

// Melody preview
extern int previewMelodyIndex;
extern bool lastSnoozed;

// Snooze & display timing
extern unsigned long messageDisplayStart;
extern time_t snoozeUntil;
extern unsigned long lastInteractionTime;

// Display object
extern Adafruit_SSD1306 display;

// Snooze timeout
extern unsigned int snoozeDurationSec;


#endif
