#include "ui/emotions/EmotionHappy.h"

void EmotionHappy::draw(Adafruit_SSD1306& display) {
  // Eyes
  display.drawCircle(40, 30, 10, SSD1306_WHITE);
  display.fillCircle(40, 30, 3, SSD1306_WHITE);
  display.drawCircle(88, 30, 10, SSD1306_WHITE);
  display.fillCircle(88, 30, 3, SSD1306_WHITE);

  // Smile
  display.drawLine(35, 45, 45, 50, SSD1306_WHITE);
  display.drawLine(45, 50, 55, 45, SSD1306_WHITE);
  display.drawLine(83, 45, 93, 50, SSD1306_WHITE);
  display.drawLine(93, 50, 103, 45, SSD1306_WHITE);
}
