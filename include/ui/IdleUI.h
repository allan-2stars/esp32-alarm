// include/ui/IdleUI.h
#pragma once

#include <Adafruit_SSD1306.h>
#include "ui_state.h"

class IdleUI {
public:
    explicit IdleUI(Adafruit_SSD1306 &display);
    void update();

private:
    Adafruit_SSD1306 &display;
    String lastTimeShown = "";
    String lastDateShown = "";

    void drawTime(const String &currentTime);
    void drawDate(const String &currentDate);
    void drawTemperatureAndHumidity();
    void drawIcons();
};
