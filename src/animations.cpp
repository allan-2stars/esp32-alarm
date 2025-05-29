#include <Arduino.h>
#include "animations.h"
#include "led.h"
#include "ui.h"
#include "config.h"
#include "light_control.h"

// This should be defined externally and updated by your UI system
extern UIState uiState;

void updateAnimations() {

  static unsigned long last = 0;
  if (millis() - last < 50) return;
  last = millis();

  struct tm timeinfo;
  getLocalTime(&timeinfo);
  int hour = timeinfo.tm_hour;
  bool isNight = (hour < 6 || hour >= 18);

  switch (uiState) {
    case IDLE_SCREEN:
      updateMoonLedFade(isNight);
      break;

    case ALARM_RINGING:
      // TODO: Pulse RGB LED red or rainbow (alarm style)
      // updateAlarmLedPulse();
      //resetAlarmLights();
      break;

    case MELODY_PREVIEW:
      // TODO: Flash LED or cycle colors with preview
      // updateMelodyLedEffect();
      break;

    case ALARM_SNOOZE_MESSAGE:
    case ALARM_OVERVIEW:
    case ALARM_CONFIG:
    case MESSAGE_DISPLAY:
    case ERROR_SCREEN:
    default:
      // No LED animation during these states for now
      updateMoonLedFade(false);  // make sure moon LED is off
      break;
  }
}

// Animate moon fade on GREEN channel
void updateMoonLedFade(bool enabled) {
  static bool fadeUp = true;
  static uint8_t brightness = 0;
  static unsigned long lastUpdate = 0;

  if (!enabled) {
    ledcWrite(CHANNEL_GREEN, 0);
    return;
  }

  if (millis() - lastUpdate >= 50) {
    lastUpdate = millis();

    if (fadeUp) {
      if (brightness < 255) brightness += 5;
      else fadeUp = false;
    } else {
      if (brightness > 0) brightness -= 5;
      else fadeUp = true;
    }

    ledcWrite(CHANNEL_GREEN, brightness);
  }
}
