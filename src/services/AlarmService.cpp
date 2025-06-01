#include "AlarmService.h"
#include "melody_engine.h"
#include "globals.h"
#include "light_control.h"
#include "ui.h"
#include "time.h"

void AlarmService::begin() {
  // Optional: startup logic for alarms
}

void AlarmService::update() {
  handleSnooze();
  checkAlarms();
}

void AlarmService::handleSnooze() {
  if (snoozeUntil > 0 && time(nullptr) >= snoozeUntil) {
    snoozeUntil = 0;
    alarmActive = true;

    startMelody(
      getMelodyData(alarms[selectedAlarmIndex].melody),
      getMelodyLength(alarms[selectedAlarmIndex].melody),
      getMelodyTempo(alarms[selectedAlarmIndex].melody),
      BUZZER_PIN, true
    );

    startAlarmLights();
    uiState = ALARM_RINGING;
  }
}

void AlarmService::checkAlarms() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  for (int i = 0; i < MAX_SCREEN_ALARMS; i++) {
    Alarm &a = alarms[i];
    if (!a.enabled) continue;
    if (a.hour != timeinfo.tm_hour || a.minute != timeinfo.tm_min) continue;
    if (timeinfo.tm_min == lastTriggerMinute) continue;

    bool shouldTrigger = false;

    if (a.type == ONE_TIME) {
      shouldTrigger = true;
      a.enabled = false;
    } else if (a.type == SPECIFIC_DATE) {
      if (a.year == (timeinfo.tm_year + 1900) &&
          a.month == (timeinfo.tm_mon + 1) &&
          a.day == timeinfo.tm_mday) {
        shouldTrigger = true;
      }
    } else if (a.type == REPEATED) {
      int weekday = timeinfo.tm_wday == 0 ? 6 : timeinfo.tm_wday - 1;
      if (a.repeatDays[weekday]) shouldTrigger = true;
    }

    if (shouldTrigger) {
      alarmActive = true;
      lastTriggerMinute = timeinfo.tm_min;

      startMelody(
        getMelodyData(a.melody),
        getMelodyLength(a.melody),
        getMelodyTempo(a.melody),
        BUZZER_PIN, true
      );

      startAlarmLights();
      uiState = ALARM_RINGING;
      break;
    }
  }
}
