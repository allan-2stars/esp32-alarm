// LedService.h
#pragma once

#include <Arduino.h>

class LedService {
public:
  void begin();
  void startAlarmLights();
  void updateAlarmLights();
  void stopAlarmLights();

private:
  unsigned long alarmStartTime = 0;
  unsigned long lastFlashUpdate = 0;
  bool flashingState = false;
  bool isAlarmActive = false;

  void setLedBrightness(int channel, int brightness);
};
