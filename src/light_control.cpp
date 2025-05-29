// light_control.cpp
#include <Arduino.h>
#include "light_control.h"
#include "config.h"

static unsigned long alarmStartTime = 0;
static unsigned long lastFlashUpdate = 0;
static bool flashingState = false;

void initAlarmLights() {
  ledcSetup(0, 5000, 8); ledcAttachPin(LED_LEFT_PIN, CHANNEL_LEFT_LED);ledcWrite(0, 0);
  ledcSetup(1, 5000, 8); ledcAttachPin(LED_TOP_PIN, CHANNEL_TOP_LED);ledcWrite(1, 0);
  ledcSetup(2, 5000, 8); ledcAttachPin(LED_RIGHT_PIN, CHANNEL_RIGHT_LED);ledcWrite(2, 0);
}

void startAlarmLights() {
  alarmStartTime = millis();
  flashingState = false;
  lastFlashUpdate = 0;
}

void resetAlarmLights() {
  ledcWrite(0, 0);
  ledcWrite(1, 0);
  ledcWrite(2, 0);
}

void updateAlarmLights() {
  unsigned long now = millis();
  unsigned long elapsed = now - alarmStartTime;

  int stage = 1;
  if (elapsed >= 240000) stage = 5;
  else if (elapsed >= 180000) stage = 4;
  else if (elapsed >= 120000) stage = 3;
  else if (elapsed >= 60000)  stage = 2;

  if (stage <= 4) {
    int fadePeriod = (stage < 4) ? 6000 : 3000;
    float phase = (now % fadePeriod) / (float)fadePeriod;
    int brightness = int(127.5 * (1.0 - cos(2 * PI * phase)));

    if (stage >= 1) ledcWrite(0, brightness);
    if (stage >= 2) ledcWrite(1, brightness);
    if (stage >= 3) ledcWrite(2, brightness);
  } else if (stage == 5) {
    if (now - lastFlashUpdate >= 1000) {
      flashingState = !flashingState;
      uint8_t level = flashingState ? 255 : 0;
      ledcWrite(0, level);
      ledcWrite(1, level);
      ledcWrite(2, level);
      lastFlashUpdate = now;
    }
  }
}
