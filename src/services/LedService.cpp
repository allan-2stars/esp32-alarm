// LedService.cpp
#include "services/LedService.h"
#include "config.h"
#include <math.h>

// Adjust channels as needed
#define CHANNEL_LEFT_LED   3
#define CHANNEL_TOP_LED    4
#define CHANNEL_RIGHT_LED  5

void LedService::begin() {
  ledcSetup(CHANNEL_LEFT_LED, 5000, 8);
  ledcAttachPin(LED_LEFT_PIN, CHANNEL_LEFT_LED);

  ledcSetup(CHANNEL_TOP_LED, 5000, 8);
  ledcAttachPin(LED_TOP_PIN, CHANNEL_TOP_LED);

  ledcSetup(CHANNEL_RIGHT_LED, 5000, 8);
  ledcAttachPin(LED_RIGHT_PIN, CHANNEL_RIGHT_LED);

  stopAlarmLights();
}

void LedService::startAlarmLights() {
  alarmStartTime = millis();
  isAlarmActive = true;
  flashingState = false;
  lastFlashUpdate = 0;
}

void LedService::stopAlarmLights() {
  isAlarmActive = false;
  ledcWrite(CHANNEL_LEFT_LED, 0);
  ledcWrite(CHANNEL_TOP_LED, 0);
  ledcWrite(CHANNEL_RIGHT_LED, 0);
}

void LedService::updateAlarmLights() {
  if (!isAlarmActive) return;

  unsigned long now = millis();
  unsigned long elapsed = now - alarmStartTime;

  int stage = 1;
  if (elapsed >= 24000) stage = 5;
  else if (elapsed >= 18000) stage = 4;
  else if (elapsed >= 12000) stage = 3;
  else if (elapsed >= 6000)  stage = 2;

  if (stage <= 4) {
    int fadePeriod = (stage < 4) ? 6000 : 3000;
    float phase = (now % fadePeriod) / (float)fadePeriod;
    int brightness = int(127.5 * (1.0 - cos(2 * PI * phase)));

    if (stage >= 1) setLedBrightness(CHANNEL_LEFT_LED, brightness);
    if (stage >= 2) setLedBrightness(CHANNEL_TOP_LED, brightness);
    if (stage >= 3) setLedBrightness(CHANNEL_RIGHT_LED, brightness);
  } else if (stage == 5) {
    if (now - lastFlashUpdate >= 1000) {
      flashingState = !flashingState;
      uint8_t level = flashingState ? 255 : 0;
      setLedBrightness(CHANNEL_LEFT_LED, level);
      setLedBrightness(CHANNEL_TOP_LED, level);
      setLedBrightness(CHANNEL_RIGHT_LED, level);
      lastFlashUpdate = now;
    }
  }
}

void LedService::setLedBrightness(int channel, int brightness) {
  ledcWrite(channel, brightness);
}
