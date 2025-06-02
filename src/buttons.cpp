#include <Arduino.h>
#include "buttons.h"
#include "alarm.h"
#include "ui.h"
#include "melodies.h"
#include "utils.h"
#include "config.h"
#include "globals.h"
#include "alarm_storage.h"
#include "light_control.h"

#include "services/MelodyService.h"
extern MelodyService melodyService;
#include "services/AlarmStorageService.h"
extern AlarmStorageService alarmStorageService;
#include "services/LedService.h"
extern LedService ledService;

extern AlarmConfigUI* alarmConfigUI;
extern Alarm alarms[3];
extern Alarm tempAlarm;
extern int selectedAlarmIndex;
extern AlarmField selectedField;
extern int currentRepeatDayIndex;
extern UIState uiState;

unsigned long lastModePress = 0;
unsigned long lastAdjustPress = 0;
unsigned long lastConfirmPress = 0;
unsigned long modeButtonPressTime = 0;

int lastTriggerMinute = -1;
bool alarmActive = false;
unsigned int snoozeDurationSec = 600;  // 10 minutes

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
  unsigned long now = millis();

  bool modePressed = digitalRead(MODE_BUTTON_PIN) == LOW;
  if (modePressed && modeButtonPressTime == 0) modeButtonPressTime = now;

  if (!modePressed && modeButtonPressTime > 0) {
    recordInteraction();
    unsigned long duration = now - modeButtonPressTime;
    melodyService.stop();
    ledService.stopAlarmLights();
    alarmActive = false;

    if (uiState == ALARM_CONFIG && alarmConfigUI) {
      alarmConfigUI->nextField();
    } else if (uiState == MELODY_PREVIEW) {
      uiState = ALARM_CONFIG;
    } else if (uiState == ALARM_RINGING) {
      snoozeUntil = time(nullptr) + snoozeDurationSec;
      lastSnoozed = true;
      uiState = ALARM_SNOOZE_MESSAGE;
      messageDisplayStart = millis();
    } else {
      uiState = (uiState == IDLE_SCREEN) ? ALARM_OVERVIEW : IDLE_SCREEN;
    }

    modeButtonPressTime = 0;
  }

  if (digitalRead(ADJUST_BUTTON_PIN) == LOW && now - lastAdjustPress > 200) {
    recordInteraction();
    melodyService.stop();
    ledService.stopAlarmLights();
    alarmActive = false;
    lastAdjustPress = now;

    if (uiState == ALARM_OVERVIEW) {
      selectedAlarmIndex = (selectedAlarmIndex + 1) % 3;
    } else if (uiState == ALARM_CONFIG && alarmConfigUI) {
      alarmConfigUI->adjustValue(true);
    } else if (uiState == MELODY_PREVIEW) {
      previewMelodyIndex = (previewMelodyIndex + 1) % MELODY_COUNT;
      melodyService.play(
        getMelodyData(previewMelodyIndex),
        getMelodyLength(previewMelodyIndex),
        getMelodyTempo(previewMelodyIndex),
        BUZZER_PIN, false
      );
    } else if (uiState == ALARM_RINGING) {
      snoozeUntil = time(nullptr) + snoozeDurationSec;
      lastSnoozed = true;
      uiState = ALARM_SNOOZE_MESSAGE;
      messageDisplayStart = millis();
    }
  }

  if (digitalRead(CONFIRM_BUTTON_PIN) == LOW && now - lastConfirmPress > 200) {
    recordInteraction();
    melodyService.stop();
    ledService.stopAlarmLights();
    alarmActive = false;
    lastConfirmPress = now;

    if (uiState == ALARM_OVERVIEW) {
      tempAlarm = alarms[selectedAlarmIndex];
      if ((tempAlarm.year == 0 || tempAlarm.month == 0 || tempAlarm.day == 0) && isTimeAvailable()) {
        setAlarmToCurrentTime(tempAlarm);
      }
      uiState = ALARM_CONFIG;
    } else if (uiState == ALARM_CONFIG && alarmConfigUI) {
      alarmConfigUI->confirm();
    } else if (uiState == MELODY_PREVIEW) {
      tempAlarm.melody = previewMelodyIndex;
      uiState = ALARM_CONFIG;
    } else if (uiState == ALARM_RINGING) {
      lastSnoozed = false;
      uiState = ALARM_SNOOZE_MESSAGE;
      messageDisplayStart = millis();
    }
  }

  if (now - lastInteraction > UI_TIMEOUT_MS && uiState != IDLE_SCREEN) {
    uiState = IDLE_SCREEN;
  }
}
