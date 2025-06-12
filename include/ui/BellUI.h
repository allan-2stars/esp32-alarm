#pragma once
#include <Adafruit_SSD1306.h>

class BellUI {
public:
  explicit BellUI(Adafruit_SSD1306 &display);
  void update();

private:
  Adafruit_SSD1306 &display;
  int frame = 0;
  unsigned long lastUpdate = 0;
};
