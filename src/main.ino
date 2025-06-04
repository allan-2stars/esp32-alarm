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
//#include "animations.h"
#include "light_control.h"
#include "sleep.h"
////

#include "services/AlarmService.h"
 AlarmService alarmService;
#include "services/MelodyService.h"
 MelodyService melodyService;
#include "services/AlarmStorageService.h"
 AlarmStorageService alarmStorageService;
#include "services/LedService.h"
 LedService ledService;
#include "services/RGBLedService.h"
 RGBLedService rgbLed;
 
#include "ui/UIManager.h"
UIManager uiManager(display);

// SunMoonUI sunMoonUI;
// MelodyPreviewUI melodyPreviewUI(display);
// AlarmOverviewUI alarmOverviewUI(display);
// AlarmConfigUI* alarmConfigUI = nullptr;
// IdleUI idleUI(display);
// #include "ui/UIManager.h"
// UIManager uiManager(display);




//UIState uiState = IDLE_SCREEN;
Alarm alarms[MAX_SCREEN_ALARMS];
Alarm tempAlarm;
int selectedAlarmIndex = 0;
// AlarmField selectedField = ALARM_TYPE;
// int currentRepeatDayIndex = 0;
// int previewMelodyIndex = 0;
bool lastSnoozed = false;
// unsigned long messageDisplayStart = 0;
time_t snoozeUntil = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
unsigned long lastFirebaseUpdate = 0;
const unsigned long firebaseInterval = 5000;  // 10 seconds

void setup() {
  Serial.begin(115200);
  //
  Wire.begin(SDA_PIN, SCL_PIN);
  initButtons();
  initBuzzer();
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

  alarmService.begin();
  melodyService.begin();
  ledService.begin();
  rgbLed.begin();
  rgbLed.setColor(255, 0, 0);  // Red
  alarmStorageService.begin();
  //melodyPreviewUI.begin(&display);
  //bellUI.begin();
  //sunMoonUI.begin();
  //alarmOverviewUI.begin(&display);
  //initAlarmLights();
  //initFirebase();
}

void loop() {
  alarmService.update();
  melodyService.update();

  handleButtons();
  resetESP32();
  //updateAnimations();
  alarmService.checkAlarms();
  uiManager.update();


  // Only call it every 10–30 seconds using millis():
  // if (millis() - lastFirebaseUpdate > firebaseInterval) {
  //   getDataFromFirebase();
  //   lastFirebaseUpdate = millis();
  // }

if (uiManager.getCurrentState() != ALARM_RINGING && millis() - lastInteraction > INACTIVITY_TIMEOUT) {
  checkIdleAndSleep();
}

  delay(50);
}

