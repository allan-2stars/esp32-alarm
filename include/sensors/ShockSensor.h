#pragma once
#include <Arduino.h>

class ShockSensor {
public:
  ShockSensor() = default;
  void begin();
  void update();
  bool isShocked() const;

private:
  static constexpr unsigned long SHOCK_TIMEOUT = 3000;

  unsigned long lastShockTime = 0;
  bool shocked = false;
};
