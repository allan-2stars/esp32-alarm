#pragma once

class ButtonHoldClassifier {
private:
  unsigned long pressStart = 0;
  unsigned long holdDuration = 0;
  bool isPressed = false;

  // Debounce
  bool stableState = false;
  bool lastRawState = false;
  unsigned long lastRawChangeTime = 0;
  const unsigned long debounceDelay = 50;
  bool releasedFlag = false;

public:
  void update(bool rawState);
  bool wasReleased() const;
  unsigned long getHoldDuration() const;
  void reset();
};
