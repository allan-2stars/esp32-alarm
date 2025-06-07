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


void initButtons() {
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
}

void handleButtons() {
  bool modeState = digitalRead(MODE_BUTTON_PIN) == LOW;
  bool adjustState = digitalRead(ADJUST_BUTTON_PIN) == LOW;
  bool confirmState = digitalRead(CONFIRM_BUTTON_PIN) == LOW;

  // === Update press states with built-in debounce ===
  confirmClassifier.update(confirmState);
  modeClassifier.update(modeState);
  adjustClassifier.update(adjustState);

  // === Mode Button ===
  if (modeClassifier.wasReleased()) {
    unsigned long dur = modeClassifier.getHoldDuration();
    if (dur >= 2000) {
      // future long press for mode
    } else if (dur >= 100) {
      uiManager.handleMode();
    }
    modeClassifier.reset();  // ✅ important
  }

  // === Adjust Button ===
  if (adjustClassifier.wasReleased()) {
    unsigned long dur = adjustClassifier.getHoldDuration();
    if (dur >= 100 && dur < repeatDelay) {
      // Normal tap
      uiManager.handleAdjust();
    }
    adjustClassifier.reset();
  } else if (adjustState) {
    // Held down
    unsigned long now = millis();
    if (adjustRepeatStart == 0) {
      adjustRepeatStart = now;
      lastAdjustRepeat = now;
    } else if (now - adjustRepeatStart >= repeatDelay && now - lastAdjustRepeat >= repeatRate) {
      uiManager.handleAdjust();  // Repeat action
      lastAdjustRepeat = now;
    }
  } else {
    // Not pressed anymore
    adjustRepeatStart = 0;
    lastAdjustRepeat = 0;
  }



// --- Confirm (based on release duration) ---
if (confirmClassifier.wasReleased()) {
  unsigned long dur = confirmClassifier.getHoldDuration();
  Serial.print("Confirm released, duration: ");
  Serial.println(dur);

  if (dur >= 1500) { // press and hold for xx seconds to swith screen
    Serial.println("Long press detected");
    UIState state = uiManager.getCurrentState();

    if (state == ROBOT_FACE_DISPLAY) {
      uiManager.switchTo(IDLE_SCREEN);
    } else if (state == IDLE_SCREEN) {
      uiManager.switchTo(ROBOT_FACE_DISPLAY);
    }
  } else if (dur >= 100) {
    //Serial.println("Short press handled");
    uiManager.handleConfirm();
  } else {
    Serial.println("Ignored tiny press");
  }

  confirmClassifier.reset();
}

}
