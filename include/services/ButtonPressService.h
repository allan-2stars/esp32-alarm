#pragma once

class ButtonPressService {
private:
  unsigned long pressStartTime = 0;
  bool isPressed = false;
  bool hasFired = false;
  unsigned long threshold;
  
  // Debounce
  bool stableState = false;
  bool lastRawState = false;
  unsigned long lastRawChangeTime = 0;
  const unsigned long debounceDelay = 50;

public:
  ButtonPressService(unsigned long thresholdMillis = 500);

  void update(bool rawState);
  bool justFired() const;
  void reset();
  void setThreshold(unsigned long newThreshold);
  bool isHolding() const;
};
