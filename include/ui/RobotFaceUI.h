// === RobotFaceUI.h ===
#pragma once

#include <Adafruit_SSD1306.h>
#include "emotions/Emotion.h"

class RobotFaceUI {
public:
  RobotFaceUI(Adafruit_SSD1306& disp);
  ~RobotFaceUI();

  void update();
  void reset();

private:
  Adafruit_SSD1306& display;
  unsigned long lastChange;
  int currentEmotion;
};