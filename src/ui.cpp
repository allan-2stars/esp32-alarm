#include <Adafruit_SSD1306.h>
#include "ui.h"
#include "alarm.h"
#include "icons.h"
#include "config.h"
#include "globals.h"
#include "melody_engine.h"
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

const char* melodyNames[MELODY_COUNT] = {
  "We Wish You", "White Xmas", "Jingle Bell", "Rudolf Red Nosed", "Santa Coming", "Silent Night"
};
const char* weekDays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

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

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(TEXT_COLOR);
  display.setCursor((SCREEN_WIDTH - 90) / 2, SCREEN_HEIGHT / 2 - 4);
  display.print("OLED Initialized");
  display.display();
  delay(1000);  // Show message briefly
  display.clearDisplay();
  display.display();
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
  getLocalTime(&timeinfo);
  int hour = timeinfo.tm_hour;  
  if (hour >= 6 && hour < 18) {
    if (millis() - lastSunAnimTime > 200) {
      sunFrameIndex = (sunFrameIndex + 1) % 5;
      lastSunAnimTime = millis();
    }
    display.drawBitmap(SCREEN_WIDTH - 16, 0, sun_frames[sunFrameIndex], 16, 16, TEXT_COLOR);
  } else {
    if (millis() - lastMoonAnimTime > 500) {
      moonVisible = !moonVisible;
      lastMoonAnimTime = millis();
    }
    if (moonVisible) {
      display.drawBitmap(SCREEN_WIDTH - 16, 0, moon_icon, 16, 16, TEXT_COLOR);
    }
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

void drawAlarmOverview() {
  display.clearDisplay();
  display.setCursor(0, 0); display.print("Alarms");
  for (int i = 0; i < MAX_SCREEN_ALARMS; i++) {
    int yPos = HEADER_HEIGHT + i * 12; // drop down leave the top line for wifi single
    display.setCursor(0, yPos);
    display.printf("%sA%d: %02d:%02d",
      i == selectedAlarmIndex ? ">" : " ",
      i + 1, alarms[i].hour, alarms[i].minute);

    // Draw bell or slashed bell
    if (alarms[i].enabled) {
      drawBellIcon(display, SCREEN_WIDTH - 10, yPos);
    } else {
      drawBellSlashIcon(display, SCREEN_WIDTH - 10, yPos);
    }
  }
  display.display();
}

void drawAlarmConfig() {
  Alarm &a = tempAlarm;
  display.clearDisplay();
  std::vector<AlarmField> visibleFields;
  if (isFieldVisible(a.type, ALARM_TYPE)) visibleFields.push_back(ALARM_TYPE);
  visibleFields.push_back(ALARM_TIME_HOUR);
  if (isFieldVisible(a.type, ALARM_DATE_YEAR)) {
    visibleFields.push_back(ALARM_DATE_YEAR);
  }
  if (isFieldVisible(a.type, ALARM_REPEAT_DAYS)) visibleFields.push_back(ALARM_REPEAT_DAYS);
  visibleFields.push_back(ALARM_ENABLED);
  visibleFields.push_back(ALARM_MELODY);

  int totalFields = visibleFields.size();
  const int maxVisibleLines = SCREEN_HEIGHT / 10 - 2;

  int selectedIndex = getSelectedFieldIndex(visibleFields);
  scrollOffset = adjustVisibleStart(selectedIndex, scrollOffset, maxVisibleLines, totalFields);

  display.setCursor(0, 0);
  display.printf("Config A%d", selectedAlarmIndex + 1);

  int drawLine = 0;
  for (int i = 0; i < totalFields; ++i) {
    if (i < scrollOffset || i >= scrollOffset + maxVisibleLines) continue;
    int y = HEADER_HEIGHT + drawLine * 10;
    AlarmField field = visibleFields[i];

    display.setCursor(0, y);
    switch (field) {
      case ALARM_TYPE:
        display.printf("%sType: %s", selectedField == ALARM_TYPE ? ">" : " ",
          a.type == ONE_TIME ? "Once" : a.type == SPECIFIC_DATE ? "Date" : "Repeat");
        break;

      case ALARM_TIME_HOUR:
        display.printf(" Time: %s%02d%s:%s%02d%s",
          selectedField == ALARM_TIME_HOUR ? "[" : "", a.hour, selectedField == ALARM_TIME_HOUR ? "]" : "",
          selectedField == ALARM_TIME_MIN ? "[" : "", a.minute, selectedField == ALARM_TIME_MIN ? "]" : "");
        break;

      case ALARM_DATE_YEAR:
        display.printf(" Date: %s%04d%s-%s%02d%s-%s%02d%s",
          selectedField == ALARM_DATE_YEAR ? "[" : "", a.year, selectedField == ALARM_DATE_YEAR ? "]" : "",
          selectedField == ALARM_DATE_MONTH ? "[" : "", a.month, selectedField == ALARM_DATE_MONTH ? "]" : "",
          selectedField == ALARM_DATE_DAY ? "[" : "", a.day, selectedField == ALARM_DATE_DAY ? "]" : "");
        break;

      case ALARM_REPEAT_DAYS: {
        display.printf("%sDays:", selectedField == ALARM_REPEAT_DAYS ? ">" : " ");
        drawLine++;
        y = HEADER_HEIGHT + drawLine * 10;
        if (y < SCREEN_HEIGHT - 8) {
          display.setCursor(0, y);
          for (int j = 0; j < 7; j++) {
            if (j == currentRepeatDayIndex && selectedField == ALARM_REPEAT_DAYS) display.print("[");
            display.print(weekDays[j][0]);
            display.print(a.repeatDays[j] ? "*" : " ");
            if (j == currentRepeatDayIndex && selectedField == ALARM_REPEAT_DAYS) display.print("]");
            display.print(" ");
          }
        }
        break;
      }

      case ALARM_ENABLED:
        display.printf("%sEnabled: %s", selectedField == ALARM_ENABLED ? ">" : " ", a.enabled ? "Yes" : "No");
        break;

      case ALARM_MELODY:
        display.printf("%sMelody: %s", selectedField == ALARM_MELODY ? ">" : " ", melodyNames[a.melody]);
        break;

      default:
        break;
    }
    drawLine++;
  }

  // if (selectedField == ALARM_MELODY && isMelodyPlaying()) {
  //   display.clearDisplay();
  //   display.setCursor(0, SCREEN_HEIGHT - 20);
  //   display.print("Previewing...");
  // }

  // Draw scroll indicator if there are more lines below
  if (scrollOffset + maxVisibleLines < totalFields) {
    display.setCursor(SCREEN_WIDTH - 6, SCREEN_HEIGHT - 8);
    display.print("~");  // Down arrow indicator
  }

  display.display();
}

void drawMelodyPreview(int selectedIndex) {
  const int melodyCount = MELODY_COUNT;

  // Adjust scroll if selection is outside view
  if (selectedIndex < scrollOffset) scrollOffset = selectedIndex;
  if (selectedIndex >= scrollOffset + visibleMelodyCount) {
    scrollOffset = selectedIndex - visibleMelodyCount + 1;
  }

  display.clearDisplay();
  display.setTextColor(TEXT_COLOR);
  display.setCursor(0, 0);
  display.print("Select Melody:");

  for (int i = 0; i < visibleMelodyCount && (i + scrollOffset) < melodyCount; i++) {
    display.setCursor(0, 12 + i * 10);
    int actualIndex = i + scrollOffset;
    display.print(actualIndex == selectedIndex ? "> " : "  ");
    display.print(melodyNames[actualIndex]);
  }

  display.setCursor(0, SCREEN_HEIGHT - 10);
  display.print("Mod:Abort, Cmf:OK");
  display.display();
}

void drawSnoozeMessage(bool wasSnoozed) {
  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(TEXT_COLOR);
  display.setCursor((SCREEN_WIDTH - 100) / 2, (SCREEN_HEIGHT / 2) - 4);
  display.print(wasSnoozed ? "Snooze for 10 mins" : "Alarm STOPPED");

  display.display();
  delay(3000);
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

