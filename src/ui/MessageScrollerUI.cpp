#include "ui/MessageScrollerUI.h"
#include "config.h"

MessageScrollerUI::MessageScrollerUI(Adafruit_SSD1306 &display, const String &message)
    : display(display), scrollOffset(0), maxLinesOnScreen((SCREEN_HEIGHT - HEADER_HEIGHT) / 10), lines() {
  splitMessage(message);
}

void MessageScrollerUI::splitMessage(const String &message) {
  int start = 0;
  while (start < message.length()) {
    int end = message.indexOf('\n', start);
    if (end == -1) end = message.length();
    lines.push_back(message.substring(start, end));
    start = end + 1;
  }
}

void MessageScrollerUI::update() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(TEXT_COLOR);
  display.setCursor(0, 0);
  display.print("Info:");

  int y = HEADER_HEIGHT;
  for (int i = scrollOffset; i < scrollOffset + maxLinesOnScreen && i < lines.size(); ++i) {
    display.setCursor(0, y);
    display.print(lines[i]);
    y += 10;
  }

  display.display();
}

void MessageScrollerUI::scrollDown() {
  if (scrollOffset + maxLinesOnScreen < lines.size()) scrollOffset++;
}

void MessageScrollerUI::scrollUp() {
  if (scrollOffset > 0) scrollOffset--;
}
