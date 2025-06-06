#include "ui/SunMoonUI.h"
#include "icons.h"
#include "config.h"

#define SUN_FRAME_COUNT 5

void SunMoonUI::begin() {
  lastSunAnimTime = millis();
  lastMoonAnimTime = millis();
}

void SunMoonUI::updateAndDraw(Adafruit_SSD1306 &display, int hour) {
  if (hour >= 6 && hour < 18) {
    // Animate sun
    if (millis() - lastSunAnimTime > 200) {
      sunFrameIndex = (sunFrameIndex + 1) % SUN_FRAME_COUNT;
      lastSunAnimTime = millis();
    }
    display.drawBitmap(SCREEN_WIDTH - 16, 0, sun_frames[sunFrameIndex], 16, 16, TEXT_COLOR);
  } else {
    // Animate moon
    if (millis() - lastMoonAnimTime > 500) {
      moonVisible = !moonVisible;
      lastMoonAnimTime = millis();
    }
    if (moonVisible) {
      display.drawBitmap(SCREEN_WIDTH - 16, 0, moon_icon, 16, 16, TEXT_COLOR);
    }
  }
}
