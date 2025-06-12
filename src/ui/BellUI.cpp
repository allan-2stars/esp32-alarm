#include "ui/BellUI.h"
#include "config.h"
#include "icons.h"

BellUI::BellUI(Adafruit_SSD1306 &display) : display(display) {}

void BellUI::update() {
  display.clearDisplay();
  int x = (SCREEN_WIDTH - 91) / 2;
  int y = (SCREEN_HEIGHT - 35) / 2;

  display.drawBitmap(x, y, wake_up_91x35, 91, 35, TEXT_COLOR);
  //display.drawBitmap(x, y, doraemon, 128, 32, TEXT_COLOR);
  display.setTextSize(1);
  display.setCursor((SCREEN_WIDTH - 120) / 2, SCREEN_HEIGHT - 12);
  display.setTextColor(TEXT_COLOR);
  display.print("Mod:Snooze, Cmf:Stop");

  display.display();
}
