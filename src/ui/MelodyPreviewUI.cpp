#include "ui/MelodyPreviewUI.h"
#include "melodies.h"
#include "config.h"

MelodyPreviewUI::MelodyPreviewUI(Adafruit_SSD1306 &disp) : display(disp) {}


void MelodyPreviewUI::draw(int selectedIndex) {
  const int melodyCount = MELODY_COUNT;
  const int visibleMelodyCount = 4;

  // Scroll logic
  if (selectedIndex < scrollOffset) scrollOffset = selectedIndex;
  if (selectedIndex >= scrollOffset + visibleMelodyCount) {
    scrollOffset = selectedIndex - visibleMelodyCount + 1;
  }

  display.clearDisplay();
  display.setTextColor(TEXT_COLOR);
  display.setCursor(0, 0);
  display.print("Select Melody:");

  for (int i = 0; i < visibleMelodyCount && (i + scrollOffset) < melodyCount; i++) {
    display.setCursor(0, HEADER_HEIGHT + i * 10);
    int actualIndex = i + scrollOffset;
    display.print(actualIndex == selectedIndex ? "> " : "  ");
    display.print(melodyNames[actualIndex]);
  }

  display.setCursor(0, SCREEN_HEIGHT - 10);
  display.print("Mod:Abort, Cmf:OK");
  display.display();
}
