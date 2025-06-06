#include "ui/MessageDisplayUI.h"
#include "config.h"
#include "globals.h"

void MessageDisplayUI::show(const String& msg) {
    lines.clear();
    scrollIndex = 0;

    display.setTextSize(1);
    int charWidth = 6;
    int charsPerLine = SCREEN_WIDTH / charWidth;
    int maxLines = (SCREEN_HEIGHT - HEADER_HEIGHT) / 8;

    String line = "";
    for (size_t i = 0; i < msg.length(); ++i) {
        if (line.length() >= charsPerLine || msg[i] == '\n') {
            lines.push_back(line);
            line = "";
        }
        if (msg[i] != '\n') line += msg[i];
    }
    if (!line.isEmpty()) lines.push_back(line);

    maxVisibleLines = maxLines;
}

void MessageDisplayUI::scrollUp() {
    if (scrollIndex > 0) scrollIndex--;
}

void MessageDisplayUI::scrollDown() {
    if (scrollIndex < (int)lines.size() - maxVisibleLines) scrollIndex++;
}

bool MessageDisplayUI::needsScroll() const {
    return lines.size() > maxVisibleLines;
}

void MessageDisplayUI::render() {
    display.clearDisplay();
    display.setCursor(0, HEADER_HEIGHT);
    display.setTextSize(1);
    display.setTextColor(TEXT_COLOR);

    for (int i = 0; i < maxVisibleLines; ++i) {
        int lineIndex = scrollIndex + i;
        if (lineIndex >= (int)lines.size()) break;
        display.setCursor(0, HEADER_HEIGHT + i * 8);
        display.println(lines[lineIndex]);
    }

    // Optional scroll indicator
    if (needsScroll()) {
        display.setCursor(SCREEN_WIDTH - 6, SCREEN_HEIGHT - 8);
        display.print(scrollIndex + maxVisibleLines < lines.size() ? "~" : "");
    }
    display.display();
}
