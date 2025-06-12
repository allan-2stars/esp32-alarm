#pragma once
#include <Adafruit_SSD1306.h>
#include "alarm.h"

class AlarmOverviewUI {
public:
  explicit AlarmOverviewUI(Adafruit_SSD1306 &display);
  void draw(const Alarm* alarms, int selectedIndex);

private:
  Adafruit_SSD1306 &display;
};
