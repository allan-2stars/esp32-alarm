#pragma once

#include <Adafruit_SSD1306.h>
#include "alarm.h"
#include "config.h"
#include "ui/UIManager.h"
#include "ui/BellUI.h"
#include "ui/IdleUI.h"
#include "ui/AlarmOverviewUI.h"
#include "ui/MelodyPreviewUI.h"
#include "ui/SunMoonUI.h"
#include "ui/AlarmConfigUI.h"
#include "services/AlarmService.h"
#include "services/MelodyService.h"
#include "services/AlarmStorageService.h"
#include "services/LedService.h"
#include "services/RGBLedService.h"

// Global display
extern Adafruit_SSD1306 display;

// Service instances
extern AlarmService alarmService;
extern MelodyService melodyService;
extern AlarmStorageService alarmStorageService;
extern LedService ledService;
extern RGBLedService rgbLed;

// UI state + managers
extern UIManager uiManager;
extern BellUI bellUI;
extern IdleUI idleUI;
extern AlarmOverviewUI alarmOverviewUI;
extern MelodyPreviewUI melodyPreviewUI;
extern SunMoonUI sunMoonUI;
extern AlarmConfigUI* alarmConfigUI;

// Alarm state
extern Alarm alarms[MAX_SCREEN_ALARMS];
extern Alarm tempAlarm;
extern int selectedAlarmIndex;
extern AlarmField selectedField;
extern int currentRepeatDayIndex;
extern int previewMelodyIndex;
extern bool lastSnoozed;
extern time_t snoozeUntil;
extern int lastTriggerMinute;

// UI and sync state
extern UIState uiState;
extern unsigned long messageDisplayStart;
extern unsigned long lastInteraction;

// Error message
extern String errorMessage;
