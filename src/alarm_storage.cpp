#include "alarm_storage.h"
#include <Preferences.h>
#include "globals.h"
#include <nvs_flash.h>
#include "utils.h"

Preferences prefs; 

void initAlarmStorage() {
  prefs.begin("alarms", false);  // namespace "alarms", read/write
  // Initialize storage
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    Serial.println("NVS corrupted. Erasing...");
    nvs_flash_erase();
    nvs_flash_init();
  }

  for (int i = 0; i < MAX_SCREEN_ALARMS; i++) {
    Alarm a;
    loadAlarm(a, i);
    // If invalid or uninitialized, use current time fallback
    if (a.version != SCREEN_ALARM_VERSION && a.version != WEB_ALARM_VERSION) {
      a = {};  // Zero all fields
      setAlarmToCurrentTime(a);
    }

    alarms[i] = a;
  }

}


void saveAlarm(const Alarm& alarm, int index) {
  char key[16];
  sprintf(key, "alarm%d", index);
  prefs.begin("alarms", false);
  prefs.putBytes(key, &alarm, sizeof(Alarm));
  prefs.end();
}


void loadAlarm(Alarm& alarm, int index) {
  char key[16];
  sprintf(key, "alarm%d", index);
  prefs.begin("alarms", true);

  Alarm temp = {};  // define temp buffer to read into
  prefs.getBytes(key, &temp, sizeof(Alarm));
  prefs.end();

  if (temp.version != SCREEN_ALARM_VERSION && temp.version != WEB_ALARM_VERSION) {
    // Invalid or uninitialized: fallback to default
    alarm = {};
    alarm.enabled = false;
    alarm.hour = 7;
    alarm.minute = 0;
    alarm.type = ONE_TIME;
    alarm.melody = 0;
    alarm.version = SCREEN_ALARM_VERSION;  // ensure it's saved correctly next time
  } else {
    alarm = temp;  // data was valid
  }
}
