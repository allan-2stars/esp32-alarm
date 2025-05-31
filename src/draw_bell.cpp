    
#include "draw_bell.h"
#include "icons.h"
#include "config.h"

// Animation state
short bellOffsetX = 0;
short bellTargetX = 0;
float bellVelocityX = 0;

// Simple bounce easing
bool ui_bounce(short *y, short *fy, float *velocity, float step) {
  float f = (*fy) - (*y);
  *velocity += f * step;
  *velocity *= 0.85;
  *y += (short)(*velocity);

  if (abs(f) < 1 && abs(*velocity) < 0.5) {
    *y = *fy;
    *velocity = 0;
    return false;
  }
  return true;
}

void drawBellRinging(Adafruit_SSD1306 &display, const String &footerMessage) {
  display.clearDisplay();

  // Animate bell X offset
  ui_bounce(&bellOffsetX, &bellTargetX, &bellVelocityX, 0.3);

  // Centered bell + horizontal offset
  int bellX = (SCREEN_WIDTH - 32) / 2 + bellOffsetX;
  int bellY = (SCREEN_HEIGHT - 32) / 2 - 8;

  display.drawBitmap(bellX, bellY, bellBitmap32x32, 32, 32, TEXT_COLOR);

  // Draw footer message
  display.setTextColor(TEXT_COLOR);
  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - footerMessage.length() * 6) / 2, SCREEN_HEIGHT - 10);
  display.print(footerMessage);

  display.display();
}
