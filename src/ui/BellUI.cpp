// BellUI.cpp
#include "BellUI.h"
#include "icons.h"
#include "config.h"

void BellUI::begin() {
  bellOffsetX = 0;
  bellTargetX = 0;
  bellVelocityX = 0;
}

void BellUI::draw(Adafruit_SSD1306 &display, const String &message) {
  display.clearDisplay();

  ui_bounce(&bellOffsetX, &bellTargetX, &bellVelocityX, 0.3);
  int bellX = (SCREEN_WIDTH - 32) / 2 + bellOffsetX;
  int bellY = (SCREEN_HEIGHT - 32) / 2 - 8;
  display.drawBitmap(bellX, bellY, bellBitmap32x32, 32, 32, TEXT_COLOR);

  display.setTextColor(TEXT_COLOR);
  display.setCursor((SCREEN_WIDTH - message.length() * 6) / 2, SCREEN_HEIGHT - 10);
  display.print(message);

  display.display();
}

void BellUI::ui_bounce(float *pos, float *target, float *velocity, float stiffness) {
  float acceleration = (*target - *pos) * stiffness;
  *velocity += acceleration;
  *velocity *= 0.8;
  *pos += *velocity;

  if (fabs(*velocity) < 0.1 && fabs(*target - *pos) < 0.1) {
    *velocity = 0;
    *pos = *target;
  }
}
