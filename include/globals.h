#pragma once
// External
#include <Face.h>
#include <Adafruit_SSD1306.h>
// Internal
#include "alarm.h"
#include "config.h"
#include "ui_state.h"
#include "ui.h"
// UI
#include "ui/UIManager.h"
#include "ui/BellUI.h"
#include "ui/IdleUI.h"
#include "ui/AlarmOverviewUI.h"
#include "ui/MelodyPreviewUI.h"
#include "ui/SunMoonUI.h"
#include "ui/AlarmConfigUI.h"
#include "ui/MessageDisplayUI.h"
#include "ui/RobotFaceUI.h"
// Services
#include "services/AlarmService.h"
#include "services/MelodyService.h"
#include "services/AlarmStorageService.h"
#include "services/LedService.h"
#include "services/RGBLedService.h"
#include "services/SleepManager.h"
#include "services/AlarmPlayerService.h"
#include "services/ButtonPressService.h"
// Commands
#include "commands/CommandInput.h"
//Animation
#include "animations/GratefulAnimation.h"
#include "animations/FaceAnimation.h"
#include "animations/SkepticAnimation.h"

extern Face *face;
// Global display
extern Adafruit_SSD1306 display;

// Service instances
extern AlarmService alarmService;
extern MelodyService melodyService;
extern AlarmStorageService alarmStorageService;
extern LedService ledService;
extern RGBLedService rgbLed;
extern AlarmPlayerService alarmPlayerService;

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
extern MessageDisplayUI messageDisplayUI;

// Sleep manager
extern SleepManager sleepManager;

// Robot
extern RobotFaceUI robotFaceUI;
extern void checkSerialCommand();
// Animation
extern GratefulAnimation gratefulAnimation;
extern SkepticAnimation skepticAnimation;
