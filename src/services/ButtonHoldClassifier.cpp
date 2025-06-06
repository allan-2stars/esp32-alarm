#include "services/ButtonHoldClassifier.h"
#include <Arduino.h>

void ButtonHoldClassifier::update(bool rawState) {
  unsigned long now = millis();

  if (stableState != rawState) {
  stableState = rawState;

  if (stableState) {
    pressStart = now;
    isPressed = true;
    holdDuration = 0;
    releasedFlag = false;
  } else if (isPressed) {
    holdDuration = now - pressStart;
    isPressed = false;
    releasedFlag = true;  // ✅ Mark as a one-time release event
  }
}

}

bool ButtonHoldClassifier::wasReleased() const {
  return !isPressed && holdDuration > 0;
}


unsigned long ButtonHoldClassifier::getHoldDuration() const {
  return holdDuration;
}

void ButtonHoldClassifier::reset() {
  holdDuration = 0;
}
