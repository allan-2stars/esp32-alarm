// #include "services/ButtonPressService.h"
// #include <Arduino.h>

// ButtonPressService::ButtonPressService(unsigned long thresholdMillis)
//   : threshold(thresholdMillis) {}

// void ButtonPressService::update(bool rawState) {
//   unsigned long now = millis();

//   if (rawState != lastRawState) {
//     lastRawChangeTime = now;
//     lastRawState = rawState;
//   }

//   if ((now - lastRawChangeTime) >= debounceDelay) {
//     if (stableState != rawState) {
//       stableState = rawState;

//       if (stableState) {
//         pressStartTime = now;
//         isPressed = true;
//         hasFired = false;
//       } else {
//         isPressed = false;
//         hasFired = false;
//       }
//     }
//   }

//   if (isPressed && !hasFired && (now - pressStartTime >= threshold)) {
//     hasFired = true;
//   }
// }

// bool ButtonPressService::justFired() const {
//   return hasFired;
// }

// void ButtonPressService::reset() {
//   hasFired = false;
// }

// void ButtonPressService::setThreshold(unsigned long newThreshold) {
//   threshold = newThreshold;
// }

// bool ButtonPressService::isHolding() const {
//   return isPressed;
// }
