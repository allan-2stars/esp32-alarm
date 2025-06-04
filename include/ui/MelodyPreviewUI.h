#pragma once
#include <Adafruit_SSD1306.h>

class MelodyPreviewUI {
public:
  explicit MelodyPreviewUI(Adafruit_SSD1306 &disp);  // NEW
  void draw(int selectedIndex);

private:
  Adafruit_SSD1306 &display;
  int scrollOffset = 0;
};
