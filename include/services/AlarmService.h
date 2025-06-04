#pragma once

#include "alarm.h"

class AlarmService {
public:
  void begin();
  void update();
  bool anyAlarmEnabled() const;
  void checkAlarms();

private:
  void handleSnooze();
  int lastTriggerMinute = -1;
};
