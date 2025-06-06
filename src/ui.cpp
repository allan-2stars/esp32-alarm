#include "ui.h"
#include "globals.h"

void initDisplay(Adafruit_SSD1306 &display) {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ SSD1306 allocation failed");
    while (true) delay(10);
  }
  uiManager.showMessage("OLED Initialized", IDLE_SCREEN, 1000);
}