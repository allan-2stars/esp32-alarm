#pragma once

#include "alarm.h"

class AlarmService {
public:
  void begin();
  void update();

private:
  void handleSnooze();
  void checkAlarms();
};
