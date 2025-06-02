// BellUI.h
#pragma once

#include <Arduino.h>
#include <Adafruit_SSD1306.h>

class BellUI {
public:
  void begin();
  void draw(Adafruit_SSD1306 &display, const String &message);

private:
  float bellOffsetX = 0;
  float bellTargetX = 0;
  float bellVelocityX = 0;

  void ui_bounce(float *pos, float *target, float *velocity, float stiffness);
};
