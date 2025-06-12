#pragma once

#include <Adafruit_SSD1306.h>
#include <vector>

class MessageDisplayUI {
public:
    void show(const String& msg);
    void scrollUp();
    void scrollDown();
    void render();
    bool needsScroll() const;

private:
    std::vector<String> lines;
    int scrollIndex = 0;
    int maxVisibleLines = 0;

};
