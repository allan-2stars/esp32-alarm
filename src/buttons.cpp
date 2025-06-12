#include <Arduino.h>
#include "buttons.h"
#include "config.h"
#include "globals.h"

// Define per-button handlers
#include "services/ButtonHoldClassifier.h"

ButtonHoldClassifier confirmClassifier;
ButtonHoldClassifier modeClassifier;
ButtonHoldClassifier adjustClassifier;

static unsigned long adjustRepeatStart = 0;
static unsigned long lastAdjustRepeat = 0;
const unsigned long repeatDelay = 500;    // Initial hold before repeat
const unsigned long repeatRate  = 150;    // Repeat every 150ms
bool adjustAllowed = true;  // Must release Adjust at least once before it's allowed again
bool confirmAllowed = true;



void initButtons() {
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
}

void handleButtons() {
  // Serial.println("$handleButtons running");

  // Read physical button states
  bool modeState = digitalRead(MODE_BUTTON_PIN) == LOW;
  bool adjustState = digitalRead(ADJUST_BUTTON_PIN) == LOW;
  bool confirmState = digitalRead(CONFIRM_BUTTON_PIN) == LOW;

  // Serial.print("modeState: "); Serial.println(modeState);
  // Serial.print("adjustState: "); Serial.println(adjustState);
  // Serial.print("confirmState: "); Serial.println(confirmState);

  // === Update all classifiers ===
  modeClassifier.update(modeState);
  adjustClassifier.update(adjustState);
  confirmClassifier.update(confirmState);

  // === Handle Mode Button ===
  if (modeClassifier.wasReleased()) {
    unsigned long dur = modeClassifier.getHoldDuration();
    //Serial.print("Mode released, duration: "); Serial.println(dur);

    if (dur >= 2000) {
      // Future long press logic
    } else if (dur >= 100) {
      uiManager.handleMode();
    }

    modeClassifier.reset();
  }

  // === Handle Adjust Button ===
  static unsigned long adjustRepeatStart = 0;
  static unsigned long lastAdjustRepeat = 0;
  const unsigned long repeatDelay = 500;
  const unsigned long repeatRate = 150;

  static bool adjustAllowed = false;
  if (!adjustState) adjustAllowed = true;  // Mark it allowed once released

  if (adjustClassifier.wasReleased() && adjustAllowed) {
    unsigned long dur = adjustClassifier.getHoldDuration();
    //Serial.print("Adjust released, duration: "); Serial.println(dur);

    if (dur >= 100 && dur < repeatDelay) {
      uiManager.handleAdjust();
    }

    adjustRepeatStart = 0;
    lastAdjustRepeat = 0;
    adjustClassifier.reset();
  } else if (adjustState && adjustAllowed) {
    // Held down
    unsigned long now = millis();
    if (adjustRepeatStart == 0) {
      adjustRepeatStart = now;
      lastAdjustRepeat = now;
    } else if (now - adjustRepeatStart >= repeatDelay && now - lastAdjustRepeat >= repeatRate) {
      uiManager.handleAdjust();
      lastAdjustRepeat = now;
    }
  } else {
    // Not pressed anymore
    adjustRepeatStart = 0;
    lastAdjustRepeat = 0;
  }

  // === Handle Confirm Button ===
  static bool confirmAllowed = false;
  if (!confirmState) confirmAllowed = true;

  if (confirmClassifier.wasReleased() && confirmAllowed) {
    unsigned long dur = confirmClassifier.getHoldDuration();
    //Serial.print("Confirm released, duration: "); Serial.println(dur);

    if (dur >= 1500) {
     // Serial.println("Long press detected");

      UIState state = uiManager.getCurrentState();
      //Serial.print("Current state: "); Serial.println(state);

      if (state == ROBOT_FACE_DISPLAY) {
        //Serial.println("Switching to IDLE_SCREEN");
        uiManager.switchTo(IDLE_SCREEN);
      } else if (state == IDLE_SCREEN) {
        //Serial.println("Switching to ROBOT_FACE_DISPLAY");
        uiManager.switchTo(ROBOT_FACE_DISPLAY);
      } else {
        //Serial.println("Long press ignored in this state.");
      }
    } else if (dur >= 100) {
      uiManager.handleConfirm();
    } else {
      //Serial.println("Ignored tiny press");
    }

    confirmClassifier.reset();
  }
}

void resetAllButtons() {
  adjustRepeatStart = 0;
  lastAdjustRepeat = 0;
  adjustAllowed = false;
  confirmAllowed = false;

  adjustClassifier.reset();
  confirmClassifier.reset();
  modeClassifier.reset();     // ✅ Add this line
}


