#include <Arduino.h>
#include <time.h>
#include <esp_sleep.h>
#include "sleep.h"
#include "config.h"
#include "globals.h"
#include "alarm.h"
#include "ui.h"
#include <Adafruit_SSD1306.h>

extern Adafruit_SSD1306 display;
extern unsigned long lastInteraction;

void checkIdleAndSleep() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  int currentSeconds = timeinfo.tm_hour * 3600 + timeinfo.tm_min * 60 + timeinfo.tm_sec;

  bool nearAlarm = false;

  // Check if any alarm is within the next 2 minutes
  for (int i = 0; i < MAX_SCREEN_ALARMS; ++i) {
    if (!alarms[i].enabled) continue;
    int alarmSeconds = alarms[i].hour * 3600 + alarms[i].minute * 60;
    int diff = alarmSeconds - currentSeconds;
    if (diff > 0 && diff <= 120) {
      nearAlarm = true;
      break;
    }
  }

  if (nearAlarm) {
    Serial.println("⏰ Alarm is too close, not going to sleep.");
    return;  // Stay awake if an alarm is within 2 minutes
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Sleeping...");
  display.display();
  delay(1000);
  display.ssd1306_command(SSD1306_DISPLAYOFF);

  // Enable only touch wakeup
  esp_sleep_enable_touchpad_wakeup();                   // Required on some platforms
  touchSleepWakeUpEnable(TOUCH_WAKE_PIN, 40);           // e.g. GPIO15 (T3)

  Serial.println("💤 Entering deep sleep. Waiting for touch...");
  esp_deep_sleep_start();
}
