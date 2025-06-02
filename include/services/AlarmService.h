#pragma once

#include "alarm.h"

class AlarmService {
public:
  void begin();
  void update();
  bool anyAlarmEnabled() const;

private:
  void handleSnooze();
  void checkAlarms();
};
