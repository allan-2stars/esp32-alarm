#include "MelodyPreviewUI.h"
#include "config.h"
#include "melodies.h"

void MelodyPreviewUI::draw(Adafruit_SSD1306& display, int selectedIndex) {
  const int melodyCount = MELODY_COUNT;
  const int visibleMelodyCount = 4;
  static int scrollOffset = 0;

  // Adjust scroll if selection is outside view
  if (selectedIndex < scrollOffset) scrollOffset = selectedIndex;
  if (selectedIndex >= scrollOffset + visibleMelodyCount) {
    scrollOffset = selectedIndex - visibleMelodyCount + 1;
  }

  display.clearDisplay();
  display.setTextColor(TEXT_COLOR);
  display.setCursor(0, 0);
  display.print("Select Melody:");

  for (int i = 0; i < visibleMelodyCount && (i + scrollOffset) < melodyCount; i++) {
    display.setCursor(0, 12 + i * 10);
    int actualIndex = i + scrollOffset;
    display.print(actualIndex == selectedIndex ? "> " : "  ");
    display.print(melodyNames[actualIndex]);
  }

  display.setCursor(0, SCREEN_HEIGHT - 10);
  display.print("Mod:Abort, Cmf:OK");
  display.display();
}
