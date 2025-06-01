//#include "alarm_storage.h"
#include <Preferences.h>
#include "globals.h"
//#include <nvs_flash.h>

#pragma once

class AlarmStorageService {
public:
  void begin();
  void saveAlarm(const Alarm &alarm, int index);
  void loadAlarm(Alarm &alarm, int index);
  void deleteAlarm(int index);
private:
  Preferences prefs;
  String getAlarmKey(int index);
};
