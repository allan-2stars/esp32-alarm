#pragma once

#include "alarm.h"

class AlarmService {
public:
  void begin();
  void update();
  bool anyAlarmEnabled() const;
  void checkAlarms();
  void handleSnooze();

private:
  int lastTriggerMinute = -1;
};
