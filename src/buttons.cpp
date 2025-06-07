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
  bool modeState = digitalRead(MODE_BUTTON_PIN) == LOW;
  bool adjustState = digitalRead(ADJUST_BUTTON_PIN) == LOW;
  bool confirmState = digitalRead(CONFIRM_BUTTON_PIN) == LOW;

  // === Update press states with built-in debounce ===
  confirmClassifier.update(confirmState);
  modeClassifier.update(modeState);
  adjustClassifier.update(adjustState);

  // ===================
  // === Mode Button ===
  // ===================
  if (modeClassifier.wasReleased()) {
    unsigned long dur = modeClassifier.getHoldDuration();
    if (dur >= 2000) {
      // future long press for mode
    } else if (dur >= 100) {
      uiManager.handleMode();
    }
    modeClassifier.reset();  // ✅ important
  }


  // =====================
  // === Adjust Button ===
  // =====================


  if (adjustClassifier.wasReleased()) {
  unsigned long dur = adjustClassifier.getHoldDuration();
  if (dur >= 100 && dur < repeatDelay) {
    if (adjustAllowed) {
      uiManager.handleAdjust();
    }
  }
  adjustAllowed = true;  // ✅ button was released, safe to use again
  adjustClassifier.reset();
  } else if (adjustState) {
  // Held down
  if (adjustAllowed) {
    unsigned long now = millis();
    if (adjustRepeatStart == 0) {
      adjustRepeatStart = now;
      lastAdjustRepeat = now;
    } else if (now - adjustRepeatStart >= repeatDelay && now - lastAdjustRepeat >= repeatRate) {
      uiManager.handleAdjust();
      lastAdjustRepeat = now;
    }
  }
  } else {
    adjustRepeatStart = 0;
    lastAdjustRepeat = 0;
  }

  // ===========================================
  // --- Confirm (based on release duration) ---
  // ===========================================
  if (confirmClassifier.wasReleased()) {
  unsigned long dur = confirmClassifier.getHoldDuration();

  UIState state = uiManager.getCurrentState();
  if (dur >= 1500 && confirmAllowed) {
   // long press in the future
  } else if (dur >= 100) {
//    if (state != ROBOT_FACE_DISPLAY) {
      uiManager.handleConfirm();
//    }
  }

  confirmAllowed = true;
  confirmClassifier.reset();
}
}

void resetAdjustRepeat() {
  adjustRepeatStart = 0;
  lastAdjustRepeat = 0;
  adjustAllowed = false;
  confirmAllowed = false;  // ✅ must release before valid again
  adjustClassifier.reset();
  confirmClassifier.reset();
}

