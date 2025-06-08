#include "globals.h"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
UIManager uiManager(display);

Alarm alarms[MAX_SCREEN_ALARMS];
Alarm tempAlarm;
int selectedAlarmIndex = 0;
int previewMelodyIndex = 0;
int currentRepeatDayIndex = 0;
AlarmField selectedField = ALARM_TYPE;
AlarmConfigUI* alarmConfigUI = nullptr;

AlarmService alarmService;
MelodyService melodyService;
AlarmStorageService alarmStorageService;
LedService ledService;
RGBLedService rgbLed;

IdleUI idleUI(display);
AlarmOverviewUI alarmOverviewUI(display);
MelodyPreviewUI melodyPreviewUI(display);
BellUI bellUI(display);
SunMoonUI sunMoonUI;

UIState uiState = IDLE_SCREEN;
bool lastSnoozed = false;
unsigned long messageDisplayStart = 0;
time_t snoozeUntil = 0;
unsigned long lastInteraction = 0;
int lastTriggerMinute = -1;

String errorMessage = "";
SleepManager sleepManager;

AlarmPlayerService alarmPlayerService;
MessageDisplayUI messageDisplayUI;

// instance RobotFaceUI
RobotFaceUI robotFaceUI;  // This creates the actual instance
GratefulAnimation gratefulAnimation;
Face *face;
