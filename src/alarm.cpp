#include <Arduino.h>
#include <time.h>
#include "melody_engine.h"
#include "alarm.h"
#include "buttons.h"  // for BUZZER_PIN
#include "config.h"
#include "ui.h"
#include "globals.h"


// External variables (defined in main.cpp or shared)
extern Alarm alarms[3];
extern int lastTriggerMinute;
extern bool alarmActive;
extern int selectedAlarmIndex;

void checkAndTriggerAlarms() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  for (int i = 0; i < 3; i++) {
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
          a.day == timeinfo.tm_mday)
        shouldTrigger = true;
    } else if (a.type == REPEATED) {
      int weekday = timeinfo.tm_wday == 0 ? 6 : timeinfo.tm_wday - 1;
      if (a.repeatDays[weekday]) shouldTrigger = true;
    }
    
    if (shouldTrigger) {
      alarmActive = true;
      lastTriggerMinute = timeinfo.tm_min;

      startMelodyPreview(
        getMelodyData(a.melody),
        getMelodyLength(a.melody),
        getMelodyTempo(a.melody),
        BUZZER_PIN
      );
      uiState = ALARM_RINGING;
      break;
    }
  }
  // Check if snooze is active and time reached
  if (snoozeUntil > 0 && time(nullptr) >= snoozeUntil) {
    snoozeUntil = 0;
    alarmActive = true;
    uiState = ALARM_RINGING;

    // Optional: reuse previously snoozed alarm melody
    startMelodyPreview(
      getMelodyData(alarms[selectedAlarmIndex].melody),
      getMelodyLength(alarms[selectedAlarmIndex].melody),
      getMelodyTempo(alarms[selectedAlarmIndex].melody),
      BUZZER_PIN
    );
  }
}

bool isFieldVisible(AlarmType type, AlarmField field) {
  if ((field == ALARM_DATE_YEAR || field == ALARM_DATE_MONTH || field == ALARM_DATE_DAY) && type != SPECIFIC_DATE)
    return false;
  if (field == ALARM_REPEAT_DAYS && type != REPEATED)
    return false;
  return true;
}
