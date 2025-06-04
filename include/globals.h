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
#include "ui/AlarmConfigUI.h"
#include "ui/IdleUI.h"

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

extern AlarmConfigUI* alarmConfigUI;

extern IdleUI idleUI;
//extern UIManager uiManager;

#endif



///

// #pragma once

// #include <Adafruit_SSD1306.h>
// #include "ui/UIManager.h"
// #include "ui/BellUI.h"
// #include "ui/IdleUI.h"
// #include "ui/AlarmOverviewUI.h"
// #include "ui/MelodyPreviewUI.h"
// #include "services/AlarmService.h"
// #include "services/MelodyService.h"
// #include "services/AlarmStorageService.h"
// #include "services/LedService.h"
// #include "services/RGBLedService.h"
// #include "ui/SunMoonUI.h"

// // Global display
// extern Adafruit_SSD1306 display;

// // Service instances
// extern AlarmService alarmService;
// extern MelodyService melodyService;
// extern AlarmStorageService alarmStorageService;
// extern LedService ledService;
// extern RGBLedService rgbLed;

// // UI instances
// extern UIManager uiManager;
// extern BellUI bellUI;
// extern IdleUI idleUI;
// extern AlarmOverviewUI alarmOverviewUI;
// extern MelodyPreviewUI melodyPreviewUI;
// extern SunMoonUI sunMoonUI;
// extern AlarmConfigUI* alarmConfigUI;

// // Shared UI state
// extern UIState uiState;
// extern Alarm alarms[MAX_SCREEN_ALARMS];
// extern Alarm tempAlarm;
// extern int selectedAlarmIndex;
// extern AlarmField selectedField;
// extern int currentRepeatDayIndex;
// extern int previewMelodyIndex;
// extern bool lastSnoozed;
// extern unsigned long messageDisplayStart;
// extern time_t snoozeUntil;
// extern unsigned long lastFirebaseUpdate;
// extern const unsigned long firebaseInterval;
// extern unsigned long lastInteraction;
// extern bool alarmActive;
// extern int lastTriggerMinute;
