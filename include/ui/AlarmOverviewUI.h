#pragma once
#include <Adafruit_SSD1306.h>
#include "alarm.h"

class AlarmOverviewUI {
public:
  void begin(Adafruit_SSD1306* oled);
  void draw(const Alarm* alarms, int selectedIndex);
private:
  Adafruit_SSD1306* display;
};
