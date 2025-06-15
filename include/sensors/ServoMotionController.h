#pragma once

#include <Arduino.h>
#include <ESP32Servo.h>
#include <vector>

class ServoMotionController {
public:
  ServoMotionController();
  void begin();
  void update();
  void setExcludedHours(const std::vector<int>& hours);

private:
  Servo servo;
  int lastHour = -1;
  unsigned long lastMotionTime = 0;
  std::vector<int> excludedHours;

  void checkHourlyTrigger();
  void checkPIRMotion();
  bool isExcludedHour(int hour);
  void turnServoTo(int angle);
};
