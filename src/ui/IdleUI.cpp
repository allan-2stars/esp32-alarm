// src/ui/IdleUI.cpp
#include "ui/IdleUI.h"
#include "sensor.h"
#include "icons.h"
#include "utils.h"
#include "config.h"
#include "services/AlarmService.h"
#include <WiFi.h>

extern AlarmService alarmService;

IdleUI::IdleUI(Adafruit_SSD1306 &display) : display(display) {}

void IdleUI::update() {
    String currentTime = getFormattedTime();  // HH:MM:SS
    String currentDate = getFormattedDate();

    float temp = getTemperature();
    float hum = getHumidity();

    bool shouldUpdate = false;
    display.setTextColor(TEXT_COLOR);

    if (currentTime != lastTimeShown) {
        lastTimeShown = currentTime;
        shouldUpdate = true;
    }

    if (abs(temp - lastTempShown) > 0.1 || abs(hum - lastHumShown) > 0.1) {
        lastTempShown = temp;
        lastHumShown = hum;
        shouldUpdate = true;

        display.fillRect(0, HEADER_HEIGHT + 22, SCREEN_WIDTH, 10, SSD1306_BLACK);
        display.setTextSize(1);
        display.setCursor(0, HEADER_HEIGHT + 22);
        display.printf("T:%s H:%s",
          isnan(lastTempShown) ? "--" : String(lastTempShown, 1).c_str(),
          isnan(lastHumShown) ? "--" : String(lastHumShown, 1).c_str()
        );
    }

    if (!shouldUpdate) return;

    display.clearDisplay();
    display.setTextColor(TEXT_COLOR);

    // WiFi animation
    int wifiStrength = WiFi.RSSI();
    const uint8_t* wifiIcon = wifi_1;
    if (wifiStrength > -67) wifiIcon = wifi_3;
    else if (wifiStrength > -70) wifiIcon = wifi_2;
    else if (wifiStrength > -80) wifiIcon = wifi_1;
    display.drawBitmap(0, 0, wifiIcon, 8, 8, TEXT_COLOR);

    // Animate Sun or Moon
    static int sunFrame = 0;
    static unsigned long lastFrameTime = 0;
    unsigned long now = millis();
    struct tm timeinfo;
    if (getLocalTime(&timeinfo)) {
        int hour = timeinfo.tm_hour;
        if (hour >= 6 && hour < 18) {
            if (now - lastFrameTime > 200) {
                sunFrame = (sunFrame + 1) % 5;
                lastFrameTime = now;
            }
            display.drawBitmap(SCREEN_WIDTH - 16, 0, sun_frames[sunFrame], 16, 16, TEXT_COLOR);
        } else {
            display.drawBitmap(SCREEN_WIDTH - 16, 0, moon_icon, 16, 16, TEXT_COLOR);
        }
    }

    // Time
    display.setTextSize(2);
    display.setCursor(5, HEADER_HEIGHT);
    display.print(currentTime);

    // Temp + Humidity
    display.setTextSize(1);
    display.setCursor(0, HEADER_HEIGHT + 22);
    display.printf("T:%s H:%s",
      isnan(lastTempShown) ? "--" : String(lastTempShown, 1).c_str(),
      isnan(lastHumShown) ? "--" : String(lastHumShown, 1).c_str()
    );

    // Alarm status icon
    display.drawBitmap(86, HEADER_HEIGHT + 22, alarmService.anyAlarmEnabled() ? bellIcon : bellSlashIcon, 8, 8, TEXT_COLOR);

    // Date
    display.setCursor(0, SCREEN_HEIGHT - 12);
    display.print(currentDate);

    display.display();
}
