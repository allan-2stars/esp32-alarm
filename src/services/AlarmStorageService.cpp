#include "../include/services/AlarmStorageService.h"
#include "globals.h"
#include "utils.h"
#include <nvs_flash.h>

void AlarmStorageService::begin() {
  prefs.begin("alarms", false);  // namespace "alarms", read/write

  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    Serial.println("NVS corrupted. Erasing...");
    nvs_flash_erase();
    nvs_flash_init();
  }

  for (int i = 0; i < MAX_SCREEN_ALARMS; i++) {
    Alarm a;
    loadAlarm(a, i);
    if (a.version != SCREEN_ALARM_VERSION && a.version != WEB_ALARM_VERSION) {
      a = {};
      setAlarmToCurrentTime(a);
    }
    alarms[i] = a;
  }
}

void AlarmStorageService::saveAlarm(const Alarm &alarm, int index) {
  String key = getAlarmKey(index);
  prefs.putBytes(key.c_str(), &alarm, sizeof(Alarm));
}

void AlarmStorageService::loadAlarm(Alarm &alarm, int index) {
  String key = getAlarmKey(index);
  size_t size = prefs.getBytes(key.c_str(), &alarm, sizeof(Alarm));
  if (size != sizeof(Alarm)) {
    alarm = {};
    setAlarmToCurrentTime(alarm);
  }
}

void AlarmStorageService::deleteAlarm(int index) {
  String key = getAlarmKey(index);
  prefs.remove(key.c_str());
}

String AlarmStorageService::getAlarmKey(int index) {
  return "a" + String(index);
}
