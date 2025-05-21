#include <Arduino.h>
#include "alarm.h"
#include "ui.h"
#include "buttons.h"
#include "melody_engine.h"
#include "utils.h"
#include "config.h"
#include "draw_bell.h"
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

Alarm alarms[3];
Alarm tempAlarm;

int selectedAlarmIndex = 0;
AlarmField selectedField = ALARM_TYPE;
int currentRepeatDayIndex = 0;
extern unsigned long lastInteractionTime;
extern int previewMelodyIndex;

UIState uiState = IDLE_SCREEN;

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

    // Auto timeout
  if (uiState != IDLE_SCREEN && millis() - lastInteractionTime > UI_TIMEOUT_MS) {
    //stopMelody();
    alarmActive = false;
    uiState = IDLE_SCREEN;
  }
    // Update display
  switch (uiState) {
    case IDLE_SCREEN: drawIdleScreen(); break;
    case ALARM_OVERVIEW: drawAlarmOverview(); break;
    case ALARM_CONFIG: drawAlarmConfig(); break;
    case MELODY_PREVIEW:
      drawMelodyPreview(previewMelodyIndex); break;
    case ALARM_RINGING: drawBellRinging(display); break;
  }

  delay(50);
}
