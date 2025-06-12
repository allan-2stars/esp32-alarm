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
    float lastTempShown = -1000;
    float lastHumShown = -1000;

    void drawTime(const String &currentTime);
    void drawDate(const String &currentDate);
    void drawTemperature(float temp);
    void drawHumidity(float hum);
    void drawIcons();
};
