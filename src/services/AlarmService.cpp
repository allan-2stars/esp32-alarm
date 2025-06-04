#include "services/AlarmService.h"
#include "globals.h"
#include "ui.h"
#include "time.h"
#include "melodies.h"
#include "services/AlarmPlayerService.h"

#include "services/MelodyService.h"
extern MelodyService melodyService;
#include "services/LedService.h"
extern LedService ledService;
#include "ui/UIManager.h"
extern UIManager uiManager;


void AlarmService::begin() {
  // Optional: startup logic for alarms
}

void AlarmService::update() {
  handleSnooze();
  checkAlarms();
}

bool AlarmService::anyAlarmEnabled() const {
    for (int i = 0; i < MAX_SCREEN_ALARMS; ++i) {
        if (alarms[i].enabled) {
            return true;
        }
    }
    return false;
}

void AlarmService::handleSnooze() {
  if (snoozeUntil > 0 && time(nullptr) >= snoozeUntil) {
    snoozeUntil = 0;
    alarmPlayerService.playAlarm(alarms[selectedAlarmIndex], true, true);
    uiManager.switchTo(ALARM_RINGING);
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
      lastTriggerMinute = timeinfo.tm_min;
      selectedAlarmIndex = i;  // Track which alarm is ringing
      alarmPlayerService.playAlarm(alarms[i], true, true);
      uiManager.switchTo(ALARM_RINGING);

      break;
    }
  }
}
