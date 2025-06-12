#include "ui/SunMoonUI.h"
#include "icons.h"
#include "config.h"

#define SUN_FRAME_COUNT 3

void SunMoonUI::begin() {
  lastSunAnimTime = millis();
  lastMoonAnimTime = millis();
}

void SunMoonUI::updateAndDraw(Adafruit_SSD1306 &display, int hour) {
  int imageWidth = 18;
  int imageHeight = 18;
  int x = SCREEN_WIDTH - imageWidth;
  int y = 0;
  if (hour >= 6 && hour < 16) {
    // Animate sun
    if (millis() - lastSunAnimTime > 200) { // rotate image every 200 milliseconds
      sunFrameIndex = (sunFrameIndex + 1) % SUN_FRAME_COUNT;
      lastSunAnimTime = millis();
    }
    display.drawBitmap(x, y, sun_frames[sunFrameIndex], imageWidth, imageHeight, TEXT_COLOR);
  } else {
    // Animate moon
    if (millis() - lastMoonAnimTime > 500) {
      moonVisible = !moonVisible;
      lastMoonAnimTime = millis();
    }
    if (moonVisible) {
      display.drawBitmap(x, y, moon_icon_18x18, imageWidth, imageHeight, TEXT_COLOR);
    }
  }
}
