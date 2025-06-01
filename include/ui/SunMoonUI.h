#pragma once
#include <Adafruit_SSD1306.h>

class SunMoonUI {
public:
  void begin();  // call once in setup()

  void updateAndDraw(Adafruit_SSD1306 &display, int hour);

private:
  unsigned long lastSunAnimTime = 0;
  unsigned long lastMoonAnimTime = 0;
  int sunFrameIndex = 0;
  bool moonVisible = true;
};
