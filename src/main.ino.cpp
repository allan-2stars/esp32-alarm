# 1 "C:\\Users\\TEST\\AppData\\Local\\Temp\\tmpgjadm8y0"
#include <Arduino.h>
# 1 "C:/Users/TEST/Documents/EA/esp32-alarm/src/main.ino"
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
 AlarmConfigUI* alarmConfigUI = nullptr;



#include "ui/BellUI.h"
BellUI bellUI;

SunMoonUI sunMoonUI;
MelodyPreviewUI melodyPreviewUI;
AlarmOverviewUI alarmOverviewUI;


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
const unsigned long firebaseInterval = 5000;
void setup();
void loop();
#line 57 "C:/Users/TEST/Documents/EA/esp32-alarm/src/main.ino"
void setup() {
  Serial.begin(115200);




  Wire.begin(SDA_PIN, SCL_PIN);
  initButtons();
  initBuzzer();
  initAHT10();
  initDisplay(display);

  if(!connectWifi()){
    display.clearDisplay();
    display.setTextColor(TEXT_COLOR);
    display.setCursor((SCREEN_WIDTH - 72) / 2, SCREEN_HEIGHT / 2 - 8);
    display.print("Internet connection failed!");
    display.display();
    return;
  }
  initNTP();

  alarmService.begin();
  melodyService.begin();
  ledService.begin();
  rgbLed.begin();
  rgbLed.setColor(255, 0, 0);
  alarmStorageService.begin();
  melodyPreviewUI.begin(&display);
  bellUI.begin();
  sunMoonUI.begin();
  alarmOverviewUI.begin(&display);


}

void loop() {
  alarmService.update();
  melodyService.update();

  handleButtons();


  resetESP32();
  updateAnimations();

  if (alarmActive) {
    uiState = ALARM_RINGING;
  }
  switch (uiState) {
    case IDLE_SCREEN: drawIdleScreen(); break;
    case ALARM_OVERVIEW: alarmOverviewUI.draw(alarms, selectedAlarmIndex); break;
    case ALARM_CONFIG:
      if (!alarmConfigUI) {
        alarmConfigUI = new AlarmConfigUI(display, &alarms[selectedAlarmIndex]);
        alarmConfigUI->begin();
      }
      if (alarmConfigUI->isDone()) {
        delete alarmConfigUI;
        alarmConfigUI = nullptr;
        uiState = ALARM_SAVE_MESSAGE;
      } else {
        alarmConfigUI->update();
      }
      break;

    case MELODY_PREVIEW: melodyPreviewUI.draw(previewMelodyIndex); break;
    case ALARM_RINGING:
      bellUI.draw(display, "Mod:Snooze, Cmf:Stop");
      ledService.updateAlarmLights();
      break;
    case ALARM_SAVE_MESSAGE:
      drawSaveAlarmMessage();
      uiState = IDLE_SCREEN;
      break;
    case ALARM_SNOOZE_MESSAGE:
      drawSnoozeMessage(lastSnoozed);
      uiState = IDLE_SCREEN;
      break;
    case ERROR_SCREEN:
      drawErrorScreen();
      break;
  }







  if (!alarmActive && millis() - lastInteraction > INACTIVITY_TIMEOUT) {
    checkIdleAndSleep();
  }
  delay(50);
}