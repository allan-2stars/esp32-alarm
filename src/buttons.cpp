#include <Arduino.h>
#include "buttons.h"
#include "alarm.h"
#include "ui.h"
#include "melodies.h"

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
unsigned long lastInteractionTime = 0;

int lastTriggerMinute = -1;
bool alarmActive = false;

void handleButtons() {
  unsigned long now = millis();

  // MODE button with short/long press support
  bool modePressed = digitalRead(MODE_BUTTON_PIN) == LOW;
  if (modePressed && modeButtonPressTime == 0) {
    modeButtonPressTime = now;
  }
  if (!modePressed && modeButtonPressTime > 0) {
    unsigned long duration = now - modeButtonPressTime;

    if (uiState == ALARM_CONFIG) {
      if (selectedField == ALARM_REPEAT_DAYS && duration > 1000) {
        for (int i = 0; i < 7; i++) tempAlarm.repeatDays[i] = false;
      } else {
        do {
          selectedField = (AlarmField)((selectedField + 1) % 10);
        } while (!isFieldVisible(tempAlarm.type, selectedField));
      }
    } else {
      uiState = (uiState == IDLE_SCREEN) ? ALARM_OVERVIEW : IDLE_SCREEN;
    }

    lastInteractionTime = now;
    modeButtonPressTime = 0;
  }

  // ADJUST button
  if (digitalRead(ADJUST_BUTTON_PIN) == LOW && now - lastAdjustPress > 200) {
    lastAdjustPress = now;
    Alarm &a = tempAlarm;
    if (uiState == ALARM_OVERVIEW) {
      selectedAlarmIndex = (selectedAlarmIndex + 1) % 3;
    } else if (uiState == ALARM_CONFIG) {
        Serial.print("Adjusting field: ");
        Serial.println(selectedField);

      switch (selectedField) {


        case ALARM_TYPE: a.type = (AlarmType)((a.type + 1) % 3); break;
        case ALARM_TIME_HOUR: a.hour = (a.hour + 1) % 24; break;
        case ALARM_TIME_MIN: a.minute = (a.minute + 1) % 60; break;
        case ALARM_DATE_YEAR: a.year = (a.year < 2035) ? a.year + 1 : 2025; break;
        case ALARM_DATE_MONTH: a.month = (a.month % 12) + 1; break;
        case ALARM_DATE_DAY: a.day = (a.day % 31) + 1; break;
        case ALARM_REPEAT_DAYS: currentRepeatDayIndex = (currentRepeatDayIndex + 1) % 7; break;
        case ALARM_ENABLED: a.enabled = !a.enabled; break;
        case ALARM_MELODY: a.melody = (a.melody + 1) % 6; break;
        default: break;
      }
    }
    lastInteractionTime = now;
  }

  // CONFIRM button
  if (digitalRead(CONFIRM_BUTTON_PIN) == LOW && now - lastConfirmPress > 200) {
    lastConfirmPress = now;
    if (uiState == ALARM_OVERVIEW) {
      tempAlarm = alarms[selectedAlarmIndex];
      uiState = ALARM_CONFIG;
      selectedField = ALARM_TYPE;
    } else if (uiState == ALARM_CONFIG) {
      if (selectedField == ALARM_REPEAT_DAYS) {
        tempAlarm.repeatDays[currentRepeatDayIndex] = !tempAlarm.repeatDays[currentRepeatDayIndex];
      } else {
        alarms[selectedAlarmIndex] = tempAlarm;
        uiState = IDLE_SCREEN;
      }
    }
    lastInteractionTime = now;
  }

  // Timeout to Idle
  if (now - lastInteractionTime > UI_TIMEOUT_MS && uiState != IDLE_SCREEN) {
    uiState = IDLE_SCREEN;
  }

  // Update display
  switch (uiState) {
    case IDLE_SCREEN: drawIdleScreen(); break;
    case ALARM_OVERVIEW: drawAlarmOverview(); break;
    case ALARM_CONFIG: drawAlarmConfig(); break;
  }
}
