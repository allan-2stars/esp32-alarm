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

  int secondsToAlarm = INACTIVITY_TIMEOUT / 1000;  // default fallback

  for (int i = 0; i < MAX_SCREEN_ALARMS; ++i) {
    if (!alarms[i].enabled) continue;
    int alarmSeconds = alarms[i].hour * 3600 + alarms[i].minute * 60;
    int diff = alarmSeconds - currentSeconds;
    if (diff > 10 && diff <= 120) {  // only sleep early if alarm is within 2 min
      secondsToAlarm = diff - 120;
      if (secondsToAlarm < 0) secondsToAlarm = 0;
      break;
    }
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Sleeping...");
  display.display();
  delay(500);
  display.ssd1306_command(SSD1306_DISPLAYOFF);

  esp_sleep_enable_touchpad_wakeup();                   // ✅ include
  touchSleepWakeUpEnable(TOUCH_WAKE_PIN, 40);           // ✅ include
  esp_sleep_enable_timer_wakeup(secondsToAlarm * 1000000ULL);

  esp_deep_sleep_start();
}
