#pragma once
#include <Adafruit_SSD1306.h>

class Emotion {
public:
  virtual void draw(Adafruit_SSD1306& display) = 0;
  virtual void update() {}
  virtual ~Emotion() = default;
};
