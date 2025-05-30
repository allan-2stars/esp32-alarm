#include <Arduino.h>
#include "buttons.h"
#include "alarm.h"
#include "ui.h"
#include "melody_engine.h"
#include "melodies.h"
#include "utils.h"
#include "config.h"
#include "globals.h"
#include "alarm_storage.h"
#include "light_control.h"

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
unsigned int snoozeDurationSec = 600;  // 10 minutes snooze time

// Initiliase Buttons on main.ino setup()
void initButtons() {
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
}

void resetESP32() {
  // This is the Test button, will be removed for real project.
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    delay(50);  // simple debounce
    ESP.restart();
  }
}

void handleButtons() {
  unsigned long now = millis();

  // MODE button with short/long press support
  //Checks if the button is currently being pressed (active LOW logic)
  bool modePressed = digitalRead(MODE_BUTTON_PIN) == LOW;
  //Detects the moment the button is first pressed down
  if (modePressed && modeButtonPressTime == 0) {
    //Starts a timer (using millis()) to track how long the button is held
    modeButtonPressTime = now;
  }
  //Detects the moment the button is released
  if (!modePressed && modeButtonPressTime > 0) {
    //Calculates how long the button was held
    unsigned long duration = now - modeButtonPressTime;
    stopMelody();

    if (uiState == ALARM_CONFIG) {
      if (selectedField == ALARM_REPEAT_DAYS && duration > 1000) {
        for (int i = 0; i < 7; i++) tempAlarm.repeatDays[i] = false;
      } else {
        do {
          selectedField = (AlarmField)((selectedField + 1) % 10);
        } while (!isFieldVisible(tempAlarm.type, selectedField));
      }
    } else if(uiState == MELODY_PREVIEW) {
      uiState = ALARM_CONFIG;
      selectedField = ALARM_MELODY;
      drawAlarmConfig();
    } else if (uiState == ALARM_RINGING) {
      alarmActive = false;
      stopMelody();
      resetAlarmLights();
      lastSnoozed = true;
      snoozeUntil = time(nullptr) + snoozeDurationSec;
      uiState = ALARM_SNOOZE_MESSAGE;
      messageDisplayStart = millis();
    }else {
      uiState = (uiState == IDLE_SCREEN) ? ALARM_OVERVIEW : IDLE_SCREEN;
    }

    recordInteraction();
    modeButtonPressTime = 0;
  }

  // ADJUST button
  if (digitalRead(ADJUST_BUTTON_PIN) == LOW && now - lastAdjustPress > 200) {
    stopMelody();
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
        case ALARM_DATE_YEAR:{
          int currentYear = getCurrentYear();
          Serial.println("Current year: ");
          Serial.print(currentYear);
          a.year = (a.year >= currentYear + 10) ? currentYear : a.year + 1;
        }break;
        case ALARM_DATE_MONTH:
          a.month = (a.month % 12) + 1;
          break;
        case ALARM_DATE_DAY:  {
          int maxDay = getMaxDay(a.year, a.month);
          a.day = (a.day % maxDay) + 1;
        }break;

        case ALARM_REPEAT_DAYS: currentRepeatDayIndex = (currentRepeatDayIndex + 1) % 7; break;
        case ALARM_ENABLED: a.enabled = !a.enabled; break;
        case ALARM_MELODY:
          uiState = MELODY_PREVIEW;
            previewMelodyIndex = alarms[selectedAlarmIndex].melody;
            // Start melody playback for first entry
            startMelody(
              getMelodyData(previewMelodyIndex),
              getMelodyLength(previewMelodyIndex),
              getMelodyTempo(previewMelodyIndex),
              BUZZER_PIN, false);
          lastAdjustPress = millis();  // avoid double-trigger
          break;
      }
    }
    else if (uiState == MELODY_PREVIEW) {
      previewMelodyIndex = (previewMelodyIndex + 1) % MELODY_COUNT;
            Serial.println("else if index:");
      Serial.println(previewMelodyIndex);
      startMelody(
        getMelodyData(previewMelodyIndex),
        getMelodyLength(previewMelodyIndex),
        getMelodyTempo(previewMelodyIndex),
        BUZZER_PIN, false);
    }
    else if (uiState == ALARM_RINGING) {
      alarmActive = false;
      stopMelody();
      resetAlarmLights();
      lastSnoozed = true;
      snoozeUntil = time(nullptr) + snoozeDurationSec;
      uiState = ALARM_SNOOZE_MESSAGE;
      messageDisplayStart = millis();
    }

    recordInteraction();
  }

  // CONFIRM button
  if (digitalRead(CONFIRM_BUTTON_PIN) == LOW && now - lastConfirmPress > 200) {
    stopMelody();
    lastConfirmPress = now;
    if (uiState == ALARM_OVERVIEW) {
      tempAlarm = alarms[selectedAlarmIndex];
      
    if ((tempAlarm.year == 0 || tempAlarm.month == 0 || tempAlarm.day == 0) && isTimeAvailable()) {
      setAlarmToCurrentTime(tempAlarm);
    }
      uiState = ALARM_CONFIG;
      selectedField = ALARM_TYPE;
    } else if (uiState == ALARM_CONFIG) {
      if (selectedField == ALARM_REPEAT_DAYS) {
        tempAlarm.repeatDays[currentRepeatDayIndex] = !tempAlarm.repeatDays[currentRepeatDayIndex];
      }else {
        alarms[selectedAlarmIndex] = tempAlarm;
        alarms[selectedAlarmIndex].version = SCREEN_ALARM_VERSION;
        saveAlarm(alarms[selectedAlarmIndex], selectedAlarmIndex);
        uiState = IDLE_SCREEN;
      }
    } else if (uiState == MELODY_PREVIEW) {
      tempAlarm.melody = previewMelodyIndex;
      uiState = ALARM_CONFIG;
      selectedField = ALARM_MELODY;
      drawAlarmConfig();
    }else if (uiState == ALARM_RINGING) {
      alarmActive = false;
      stopMelody();
      resetAlarmLights();
      lastSnoozed = false;
      uiState = ALARM_SNOOZE_MESSAGE;
      messageDisplayStart = millis();
    }
    recordInteraction();
    lastInteractionTime = now;
  }

  // Timeout to Idle
  if (now - lastInteraction > UI_TIMEOUT_MS && uiState != IDLE_SCREEN) {
    uiState = IDLE_SCREEN;
  }


}


