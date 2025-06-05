#pragma once
#include <Adafruit_SSD1306.h>
#include <vector>

class MessageScrollerUI {
public:
  MessageScrollerUI(Adafruit_SSD1306 &display, const String &message);
  void update();
  void scrollUp();
  void scrollDown();
  void showScrollableMessage(const String &message);


private:
  Adafruit_SSD1306 &display;
  std::vector<String> lines;
  int scrollOffset;
  int maxLinesOnScreen;
  void splitMessage(const String &message);
  class MessageScrollerUI* scrollableMessageUI;  // Pointer version

};



