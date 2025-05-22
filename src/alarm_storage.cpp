#include "alarm_storage.h"
#include <Preferences.h>


void saveAlarm(const Alarm& alarm, int index) {
  Preferences prefs;
  char key[16];
  sprintf(key, "alarm%d", index);
  prefs.begin("alarms", false);
  prefs.putBytes(key, &alarm, sizeof(Alarm));
  prefs.end();
}


void loadAlarm(Alarm& alarm, int index) {
  Preferences prefs;
  char key[16];
  sprintf(key, "alarm%d", index);
  prefs.begin("alarms", true);

  Alarm temp = {};  // define temp buffer to read into
  prefs.getBytes(key, &temp, sizeof(Alarm));
  prefs.end();

  if (temp.version != DEFAULT_ALARM_VERSION) {
    // Invalid or uninitialized: fallback to default
    alarm = {};
    alarm.enabled = false;
    alarm.hour = 7;
    alarm.minute = 0;
    alarm.type = ONE_TIME;
    alarm.melody = 0;
    alarm.version = DEFAULT_ALARM_VERSION;  // ensure it's saved correctly next time
  } else {
    alarm = temp;  // data was valid
  }
}
