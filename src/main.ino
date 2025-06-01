#include <Arduino.h>
#include "alarm.h"
#include "ui.h"
#include "buttons.h"

#include "utils.h"
#include "config.h"
#include "draw_bell.h"
#include "globals.h"
#include "alarm_storage.h"
#include "led.h"
#include "web_server.h"
#include "secrets.h"
#include "sensor.h"
#include "alarm_storage.h"
#include "animations.h"
#include "light_control.h"
#include "sleep.h"
////
#include "services/AlarmService.h"
AlarmService alarmService;
#include "services/MelodyService.h"
MelodyService melodyService;

UIState uiState = IDLE_SCREEN;
Alarm alarms[MAX_SCREEN_ALARMS];
Alarm tempAlarm;
int selectedAlarmIndex = 0;
AlarmField selectedField = ALARM_TYPE;
int currentRepeatDayIndex = 0;
int previewMelodyIndex = 0;
bool lastSnoozed = false;
unsigned long messageDisplayStart = 0;
time_t snoozeUntil = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
unsigned long lastFirebaseUpdate = 0;
const unsigned long firebaseInterval = 5000;  // 10 seconds

void setup() {
  Serial.begin(115200);
  alarmService.begin();
  melodyService.begin();

  Wire.begin(SDA_PIN, SCL_PIN);
  initButtons();
  initBuzzer();
  initLED();
  initRGBLed();
  initAHT10();
  initDisplay(display);
  // Connect to Wifi
  if(!connectWifi()){
    display.clearDisplay();
    display.setTextColor(TEXT_COLOR);
    display.setCursor((SCREEN_WIDTH - 72) / 2, SCREEN_HEIGHT / 2 - 8);
    display.print("Internet connection failed!");
    display.display(); // make a dedicated display error message;
    return;
  }
  initNTP();
  initAlarmStorage();
  initAlarmLights();
  //initFirebase();
}

void loop() {
  alarmService.update();
  melodyService.update();

  handleButtons();
  //checkAndTriggerAlarms();
  //updateMelodyPlayback();
  resetESP32();
  updateAnimations();
  // ✅ Force uiState to ALARM_RINGING while active
  if (alarmActive) {
    uiState = ALARM_RINGING;
  }
  switch (uiState) {
    case IDLE_SCREEN: drawIdleScreen(); break;
    case ALARM_OVERVIEW: drawAlarmOverview(); break;
    case ALARM_CONFIG: drawAlarmConfig(); break;
    case MELODY_PREVIEW: drawMelodyPreview(previewMelodyIndex); break;
    case ALARM_RINGING:
      drawBellRinging(display, "Mod:Snooze, Cmf:Stop");
      updateAlarmLights();  // ✅ uncomment this now
      break;
    case ALARM_SNOOZE_MESSAGE:
      drawSnoozeMessage(lastSnoozed);
      uiState = IDLE_SCREEN;
      break;
    case ERROR_SCREEN:
      drawErrorScreen();
      break;
  }

  // Only call it every 10–30 seconds using millis():
  // if (millis() - lastFirebaseUpdate > firebaseInterval) {
  //   getDataFromFirebase();
  //   lastFirebaseUpdate = millis();
  // }

  if (!alarmActive && millis() - lastInteraction > INACTIVITY_TIMEOUT) {
    checkIdleAndSleep();
  }
  delay(50);
}

