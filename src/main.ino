#include <Arduino.h>
#include "alarm.h"
#include "ui.h"
#include "buttons.h"
#include "melody_engine.h"
#include "utils.h"
#include "config.h"
#include "draw_bell.h"
#include "globals.h"
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//####
UIState uiState = IDLE_SCREEN;
Alarm alarms[3];
Alarm tempAlarm;
int selectedAlarmIndex = 0;
AlarmField selectedField = ALARM_TYPE;
int currentRepeatDayIndex = 0;
int previewMelodyIndex = 0;
bool lastSnoozed = false;
unsigned long messageDisplayStart = 0;
time_t snoozeUntil = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//####

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  // Connect Wi-Fi
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" WiFi connected");

  // Set timezone and NTP
  configTzTime("AEST-10AEDT,M10.1.0,M4.1.0/3", "pool.ntp.org");

  // ⏱️ Wait for NTP sync
  struct tm timeinfo;

  unsigned long startTime = millis();
  bool timeSynced = false;
  while (millis() - startTime < 10000) {
    if (getLocalTime(&timeinfo) && timeinfo.tm_year + 1900 >= 2024) {
      timeSynced = true;
      break;
    }
    delay(200);
    Serial.println("Waiting for NTP sync...");
  }
  if (timeSynced) {
    Serial.println("Time synced!");
  } else {
    Serial.println("NTP timeout — using fallback time.");
  }

  // while (!getLocalTime(&timeinfo) || timeinfo.tm_year + 1900 < 2024) {
  //   delay(200);
  //   Serial.println("Waiting for time sync...");
  // }
  // Serial.println("Time synced!");

  // after NTP wait make sure alarm time is synced
  for (int i = 0; i < 3; i++) {
    alarms[i].enabled = false;
    alarms[i].type = ONE_TIME;
    setAlarmToCurrentTime(alarms[i]);
  }

}

void loop() {
  handleButtons();          // input + screen update
  checkAndTriggerAlarms();  // time match + melody
  updateMelodyPlayback();

  // if (uiState == ALARM_SNOOZE_MESSAGE && millis() - messageDisplayStart > 3000) {
  //   uiState = IDLE_SCREEN;
  // }

    // Update display
  switch (uiState) {
    case IDLE_SCREEN: drawIdleScreen(); break;
    case ALARM_OVERVIEW: drawAlarmOverview(); break;
    case ALARM_CONFIG: drawAlarmConfig(); break;
    case MELODY_PREVIEW: drawMelodyPreview(previewMelodyIndex); break;
    case ALARM_RINGING: drawBellRinging(display); break;
    // if lastSnoozed is true, then show snooze message, otherwise, show stop message.
    case ALARM_SNOOZE_MESSAGE: {
      drawSnoozeMessage(lastSnoozed); 
      uiState = IDLE_SCREEN;break;
    }
  }

  delay(50);
}
