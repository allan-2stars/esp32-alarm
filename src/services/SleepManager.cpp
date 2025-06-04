#include "services/SleepManager.h"
#include <Arduino.h>
#include <time.h>
#include <esp_sleep.h>
#include "config.h"
#include "globals.h"
#include "alarm.h"
#include "ui.h"

void SleepManager::begin() {
  // Nothing for now – placeholder for future expansion
}

void SleepManager::update() {
  if (uiManager.getCurrentState() == ALARM_RINGING) return;

  if (millis() - lastInteraction > INACTIVITY_TIMEOUT) {
    if (!isNearAlarm()) {
      enterSleep();
    } else {
      Serial.println("⏰ Alarm is too close, not going to sleep.");
    }
  }
}

bool SleepManager::isNearAlarm() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return false;

  int currentSeconds = timeinfo.tm_hour * 3600 + timeinfo.tm_min * 60 + timeinfo.tm_sec;

  for (int i = 0; i < MAX_SCREEN_ALARMS; ++i) {
    if (!alarms[i].enabled) continue;
    int alarmSeconds = alarms[i].hour * 3600 + alarms[i].minute * 60;
    int diff = alarmSeconds - currentSeconds;
    if (diff > 0 && diff <= 120) return true;
  }
  return false;
}

void SleepManager::enterSleep() {
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Sleeping...");
  display.display();
  delay(1000);
  display.ssd1306_command(SSD1306_DISPLAYOFF);

  esp_sleep_enable_touchpad_wakeup();
  touchSleepWakeUpEnable(TOUCH_WAKE_PIN, 40);

  Serial.println("💤 Entering deep sleep. Waiting for touch...");
  esp_deep_sleep_start();
}
