#include <Adafruit_SSD1306.h>
#include "ui.h"
#include "alarm.h"
#include "icons.h"
#include "config.h"
#include "globals.h"
#include "melodies.h"
#include "utils.h"
#include "sensor.h"
#include <time.h>

extern Adafruit_SSD1306 display;
extern Alarm alarms[MAX_SCREEN_ALARMS];
extern Alarm tempAlarm;
extern int selectedAlarmIndex;
extern AlarmField selectedField;
extern int currentRepeatDayIndex;
extern UIState uiState;
extern bool lastSnoozed;

int scrollOffset = 0;
const int visibleMelodyCount = 4;




String lastTimeShown = "";
float lastTempShown = -1000;   // impossible value to force first update
float lastHumShown = -1000;


unsigned long lastWifiAnimTime = 0;
int wifiAnimFrame = 0;

unsigned long lastSunAnimTime = 0;
int sunFrameIndex = 0;

unsigned long lastMoonAnimTime = 0;
bool moonVisible = true;

int getSelectedFieldIndex(const std::vector<AlarmField> &visibleFields) {
  for (size_t i = 0; i < visibleFields.size(); ++i) {
    if (visibleFields[i] == selectedField) return i;
  }
  return 0;
}

void initDisplay(Adafruit_SSD1306 &display) {
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("❌ SSD1306 allocation failed");
    while (true) delay(10);
  }
  uiManager.showMessage("OLED Initialized", IDLE_SCREEN, 1000);
}

void drawWiFiAnimation() {
    // Animate Wi-Fi (top-left)
  if (millis() - lastWifiAnimTime > 1000) {
    wifiAnimFrame = (wifiAnimFrame + 1) % 3;
    lastWifiAnimTime = millis();
  }
  const unsigned char* wifiFrames[] = {wifi_1, wifi_2, wifi_3};
  display.drawBitmap(0, 0, wifiFrames[wifiAnimFrame], 8, 8, TEXT_COLOR);
}

void drawIdleScreen() {
  String currentTime = getFormattedTime();  // HH:MM:SS
  String currentDate = getFormattedDate();

  // Get temperature and humidity
  float temp = getTemperature();
  float hum = getHumidity();

  bool shouldUpdate = false;
  display.setTextColor(TEXT_COLOR);

  // Detect time change
  if (currentTime != lastTimeShown) {
    lastTimeShown = currentTime;
    shouldUpdate = true;
  }

  // Detect temp/hum changes
  if (abs(temp - lastTempShown) > 0.1 || abs(hum - lastHumShown) > 0.1) {
    lastTempShown = temp;
    lastHumShown = hum;
    shouldUpdate = true;

    display.fillRect(0, HEADER_HEIGHT + 22, SCREEN_WIDTH, 10, SSD1306_BLACK);  // clear only sensor area
    display.setTextSize(1);
    display.setCursor(0, HEADER_HEIGHT + 22);
    display.printf("T:%s H:%s",
      isnan(lastTempShown) ? "--" : String(lastTempShown, 1).c_str(),
      isnan(lastHumShown) ? "--" : String(lastHumShown, 1).c_str()
    );
  }

  if (!shouldUpdate) return;

  // If we reached here, something changed
  display.clearDisplay();
  display.setTextColor(TEXT_COLOR);

  drawWiFiAnimation();

  // Animate Sun or Moon (top-right)
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    sunMoonUI.updateAndDraw(display, timeinfo.tm_hour);
  }

  // Time (always shown if updated)
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

  // Alarm status
  bool enabled = false;
  for (int i = 0; i < 3; i++) if (alarms[i].enabled) enabled = true;
  display.setCursor(86, HEADER_HEIGHT + 22);
  display.print(enabled ? "A:ON" : "A:OFF");

  // Date
  display.setCursor(0, SCREEN_HEIGHT - 12);
  display.print(currentDate);

  display.display();  // push to screen
}

void drawSnoozeMessage(bool wasSnoozed) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(TEXT_COLOR);
  display.setCursor((SCREEN_WIDTH - 100) / 2, (SCREEN_HEIGHT / 2) - 4);
  display.print(wasSnoozed ? "Snooze for 10 mins" : "Alarm STOPPED");

  display.display();
  //delay(2000);
}

void drawSaveAlarmMessage() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(TEXT_COLOR);
  display.setCursor((SCREEN_WIDTH - 100) / 2, (SCREEN_HEIGHT / 2) - 4);
  display.print("Alarm Saved!");

  display.display();
  //delay(2000);
}

void drawErrorScreen() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(TEXT_COLOR);

  int y = 10;
  int start = 0;
  while (start < errorMessage.length()) {
    int nl = errorMessage.indexOf('\n', start);
    if (nl == -1) nl = errorMessage.length();
    String line = errorMessage.substring(start, nl);
    display.setCursor(10, y);
    display.println(line);
    y += 8;
    start = nl + 2;
  }

  display.display();
}

