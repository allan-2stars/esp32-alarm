// MelodyPreviewUI.h
#pragma once
#include <Adafruit_SSD1306.h>

class MelodyPreviewUI {
public:
  void draw(Adafruit_SSD1306& display, int selectedIndex);
};
