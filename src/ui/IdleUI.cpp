// src/ui/IdleUI.cpp
#include "ui/IdleUI.h"
#include "sensor.h"
#include "icons.h"
#include "utils.h"
#include "config.h"
#include "services/AlarmService.h"
#include <WiFi.h>
#include "globals.h"

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

    struct tm timeinfo;
   
    if (getLocalTime(&timeinfo)) {
      sunMoonUI.updateAndDraw(display, timeinfo.tm_hour);
    }
    else {
      Serial.println("not getting the real time.");
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
