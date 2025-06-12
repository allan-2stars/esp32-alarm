#pragma once
#include <Arduino.h>

class ShockSensor {
public:
  ShockSensor() = default;
  void begin();
  void update();
  bool isShocked() const;

private:
  //static constexpr uint8_t SENSOR_PIN = 13;  // KY-002 connected here
  static constexpr unsigned long SHOCK_TIMEOUT = 2000;  // LED/display/response duration

  unsigned long lastShockTime = 0;
  bool shocked = false;
};
