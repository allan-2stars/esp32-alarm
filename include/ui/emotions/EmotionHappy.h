#pragma once
#include "Emotion.h"

class EmotionHappy : public Emotion {
public:
  void draw(Adafruit_SSD1306& display) override;
};
