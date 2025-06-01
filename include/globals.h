#ifndef GLOBALS_H
#define GLOBALS_H

#include <time.h>
#include <Adafruit_SSD1306.h>
#include "alarm.h"
#include "ui.h"
#include <Preferences.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include "ui/SunMoonUI.h"
#include "ui/MelodyPreviewUI.h"
#include "ui/AlarmOverviewUI.h"

// Shared system state
extern UIState uiState;
extern Alarm alarms[MAX_SCREEN_ALARMS];
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

// Display object
extern Adafruit_SSD1306 display;

// Snooze timeout
extern unsigned int snoozeDurationSec;

// store and display the actual message
extern String errorMessage;

// store alarms in memory
extern Preferences prefs;

// Firebase
// Declare external shared objects (defined in main.ino or globals.cpp)
extern WiFiClientSecure ssl_client;
extern AsyncClientClass aClient;
extern FirebaseApp app;
extern RealtimeDatabase Database;
extern UserAuth user_auth;
extern unsigned long lastDataCheck;
//
extern SunMoonUI sunMoonUI;
extern MelodyPreviewUI melodyPreviewUI;
extern AlarmOverviewUI alarmOverviewUI;



#endif
