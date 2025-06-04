#include <Arduino.h>
#include "buttons.h"
#include "config.h"
#include "ui/UIManager.h"
// #include "globals.h"

extern UIManager uiManager;

void initButtons() {
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
}

void resetESP32() {
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    delay(50);
    ESP.restart();
  }
}

void handleButtons() {
  static unsigned long lastDebounceTime = 0;
  static bool lastModeState = HIGH;
  static bool lastAdjustState = HIGH;
  static bool lastConfirmState = HIGH;
  static unsigned long adjustPressStart = 0;
  unsigned long lastAdjustRepeat = 0;
  bool adjustHeld = false;


  unsigned long now = millis();
  bool modeState = digitalRead(MODE_BUTTON_PIN);
  bool adjustState = digitalRead(ADJUST_BUTTON_PIN);
  bool confirmState = digitalRead(CONFIRM_BUTTON_PIN);

  // Debounce interval
  const unsigned long debounceDelay = 200;

  // Mode button
  if (lastModeState == HIGH && modeState == LOW && (now - lastDebounceTime > debounceDelay)) {
    uiManager.handleMode();
    lastDebounceTime = now;
  }

  // === Adjust button auto-repeat ===
  if (adjustState == LOW) {
    if (lastAdjustState == HIGH) {
      // Just pressed
      adjustPressStart = millis();
      lastAdjustRepeat = millis();
      uiManager.handleAdjust();  // Initial tap
    } else {
      // Held down
      if (millis() - adjustPressStart > 500 && millis() - lastAdjustRepeat > 150) {
        uiManager.handleAdjust();  // Repeated action
        lastAdjustRepeat = millis();
      }
    }
  } else {
    adjustPressStart = 0;
  }
  lastAdjustState = adjustState;


  // Confirm button
  if (lastConfirmState == HIGH && confirmState == LOW && (now - lastDebounceTime > debounceDelay)) {
    uiManager.handleConfirm();
    lastDebounceTime = now;
  }

  lastModeState = modeState;
  lastAdjustState = adjustState;
  lastConfirmState = confirmState;
}
