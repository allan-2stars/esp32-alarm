// light_control.cpp
#include <Arduino.h>
#include "light_control.h"
#include "config.h"

static unsigned long alarmStartTime = 0;
static unsigned long lastFlashUpdate = 0;
static bool flashingState = false;
bool animationActive = false;


void initAlarmLights() {
  ledcSetup(CHANNEL_LEFT_LED, 5000, 8); ledcAttachPin(LED_LEFT_PIN, CHANNEL_LEFT_LED);
  ledcSetup(CHANNEL_TOP_LED, 5000, 8); ledcAttachPin(LED_TOP_PIN, CHANNEL_TOP_LED);
  ledcSetup(CHANNEL_RIGHT_LED, 5000, 8); ledcAttachPin(LED_RIGHT_PIN, CHANNEL_RIGHT_LED);
}

void startAlarmLights() {
  animationActive = true;
  alarmStartTime = millis();
  lastFlashUpdate = 0;
  flashingState = false;
}

void resetAlarmLights() {
  animationActive = false;
  ledcWrite(CHANNEL_LEFT_LED, 0);
  ledcWrite(CHANNEL_TOP_LED, 0);
  ledcWrite(CHANNEL_RIGHT_LED, 0);
}

void updateAlarmLights() {
  if (!animationActive) {
    return;
  }

  unsigned long now = millis();
  unsigned long elapsed = now - alarmStartTime;

  int stage = 1;
  if (elapsed >= 24000) stage = 5;
  else if (elapsed >= 18000) stage = 4;
  else if (elapsed >= 12000) stage = 3;
  else if (elapsed >= 2000)  stage = 2;

  Serial.print("Elapsed: ");
  Serial.print(elapsed);
  Serial.print("ms, Stage: ");
  Serial.println(stage);

  if (stage <= 4) {
    int fadePeriod = (stage < 4) ? 6000 : 3000;
    float phase = (now % fadePeriod) / (float)fadePeriod;
    int brightness = int(127.5 * (1.0 - cos(2 * PI * phase)));

    if (stage >= 1) ledcWrite(CHANNEL_LEFT_LED, brightness);
    if (stage >= 2) ledcWrite(CHANNEL_TOP_LED, brightness);
    if (stage >= 3) ledcWrite(CHANNEL_RIGHT_LED, brightness);
  } else if (stage == 5) {
    if (now - lastFlashUpdate >= 1000) {
      flashingState = !flashingState;
      uint8_t level = flashingState ? 255 : 0;

      ledcWrite(CHANNEL_LEFT_LED, level);
      ledcWrite(CHANNEL_TOP_LED, level);
      ledcWrite(CHANNEL_RIGHT_LED, level);
      lastFlashUpdate = now;
    }
  }
}

