#include "ui/BellUI.h"
#include "config.h"
#include "icons.h"

BellUI::BellUI(Adafruit_SSD1306 &display) : display(display) {}

void BellUI::update() {
  int imageWidth = 39;
  int imageHeight = 35;
  display.clearDisplay();
  int x = (SCREEN_WIDTH - imageWidth) / 2;
  int y = (SCREEN_HEIGHT - imageHeight) / 2;

  display.drawBitmap(x, y, bell_ringing_39x35, imageWidth, imageHeight, TEXT_COLOR);
  //display.drawBitmap(x, y, doraemon, 128, 32, TEXT_COLOR);
  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - 120) / 2, SCREEN_HEIGHT - 12);
  display.setTextColor(TEXT_COLOR);
  display.print("Mod:Snooze, Cmf:Stop");

  display.display();
}
