#include "ui/BellUI.h"
#include "config.h"
#include "icons.h"

BellUI::BellUI(Adafruit_SSD1306 &display) : display(display) {}

void BellUI::update() {
  display.clearDisplay();

  int x = (SCREEN_WIDTH - 32) / 2;
  int y = (SCREEN_HEIGHT - 32) / 2;

  display.drawBitmap(x, y, bellBitmap32x32, 32, 32, TEXT_COLOR);

  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - 80) / 2, SCREEN_HEIGHT - 12);
  display.setTextColor(TEXT_COLOR);
  display.print("Mod:Snooze, Cmf:Stop");

  display.display();
}
