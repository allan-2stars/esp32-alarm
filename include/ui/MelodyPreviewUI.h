#pragma once
#include <Adafruit_SSD1306.h>

class MelodyPreviewUI {
public:
  void begin(Adafruit_SSD1306* disp);
  void draw(int selectedIndex);

private:
  Adafruit_SSD1306* display;
  int scrollOffset = 0;
};
