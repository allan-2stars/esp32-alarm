#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <time.h>
#include "melodies.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SDA_PIN 16
#define SCL_PIN 17

#define DHTPIN 14
#define DHTTYPE DHT22

#define MODE_BUTTON_PIN 33
#define ADJUST_BUTTON_PIN 5
#define CONFIRM_BUTTON_PIN 4
#define BUZZER_PIN 15
#define UI_TIMEOUT_MS 60000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

enum UIState { IDLE_SCREEN, ALARM_OVERVIEW, ALARM_CONFIG };
enum AlarmField {
  ALARM_TYPE, ALARM_TIME_HOUR, ALARM_TIME_MIN,
  ALARM_DATE_YEAR, ALARM_DATE_MONTH, ALARM_DATE_DAY,
  ALARM_REPEAT_DAYS, ALARM_ENABLED, ALARM_MELODY,
  ALARM_SAVE_EXIT
};
enum AlarmType { ONE_TIME, SPECIFIC_DATE, REPEATED };

struct Alarm {
  bool enabled = false;
  AlarmType type = ONE_TIME;
  int hour = 7, minute = 0, year = 2025, month = 5, day = 18;
  bool repeatDays[7] = {false};
  int melody = 0;
  String name = "";
};

Alarm alarms[3], tempAlarm;
int selectedAlarmIndex = 0;
AlarmField selectedField = ALARM_TYPE;
int currentRepeatDayIndex = 0;
UIState uiState = IDLE_SCREEN;
unsigned long lastInteractionTime = 0;
unsigned long lastModePress = 0, lastAdjustPress = 0, lastConfirmPress = 0;
unsigned long modeButtonPressTime = 0;
int lastTriggerMinute = -1;
bool alarmActive = false;


const char* melodyNames[] = {"Mario", "Birthday", "SilentNight"};
const char* weekDays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

bool isFieldVisible(AlarmType type, AlarmField field) {
  if ((field == ALARM_DATE_YEAR || field == ALARM_DATE_MONTH || field == ALARM_DATE_DAY) && type != SPECIFIC_DATE) return false;
  if (field == ALARM_REPEAT_DAYS && type != REPEATED) return false;
  return true;
}

void connectToWiFi() {
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) delay(100);
  configTzTime("AEST-10AEDT,M10.1.0,M4.1.0/3", "pool.ntp.org");
}

String getFormattedTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "Time Err";
  char buf[6]; strftime(buf, sizeof(buf), "%H:%M", &timeinfo);
  return String(buf);
}

String getFormattedDate() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "Date Err";
  char buf[20]; strftime(buf, sizeof(buf), "%a %Y-%m-%d", &timeinfo);
  return String(buf);
}

void drawIdleScreen() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setCursor(0, 0);
  display.print(getFormattedTime());

  display.setTextSize(1);
  display.setCursor(0, 20);
  display.printf("T:%.1fC H:%.1f%%", dht.readTemperature(), dht.readHumidity());

  display.setCursor(0, 32);
  bool enabled = false;
  for (int i = 0; i < 3; i++) if (alarms[i].enabled) enabled = true;
  display.print(enabled ? "Alarm ON" : "Alarm OFF");

  display.setCursor(0, 52);
  display.print(getFormattedDate());
  display.display();
}

void drawAlarmOverview() {
  display.clearDisplay();
  display.setCursor(0, 0); display.print("Alarms");
  for (int i = 0; i < 3; i++) {
    display.setCursor(0, 12 + i * 16);
    display.printf("%sA%d: %02d:%02d %s",
      i == selectedAlarmIndex ? ">" : " ",
      i + 1, alarms[i].hour, alarms[i].minute,
      alarms[i].enabled ? "ON" : "OFF");
  }
  display.display();
}

void drawAlarmConfig() {
  Alarm &a = tempAlarm;
  display.clearDisplay();
  int y = 0;
  display.setCursor(0, y); y += 10;
  display.printf("Config A%d", selectedAlarmIndex + 1);

  if (isFieldVisible(a.type, ALARM_TYPE)) {
    display.setCursor(0, y); y += 10;
    display.printf("%sType: %s", selectedField == ALARM_TYPE ? ">" : " ", a.type == ONE_TIME ? "Once" : a.type == SPECIFIC_DATE ? "Date" : "Repeat");
  }

  display.setCursor(0, y); y += 10;
  display.printf("%sTime: %02d:%02d", (selectedField == ALARM_TIME_HOUR || selectedField == ALARM_TIME_MIN) ? ">" : " ", a.hour, a.minute);

  if (isFieldVisible(a.type, ALARM_DATE_YEAR)) {
    display.setCursor(0, y); y += 10;
    display.printf("%sDate: %04d-%02d-%02d", (selectedField == ALARM_DATE_YEAR || selectedField == ALARM_DATE_MONTH || selectedField == ALARM_DATE_DAY) ? ">" : " ", a.year, a.month, a.day);
  }

  if (isFieldVisible(a.type, ALARM_REPEAT_DAYS)) {
    display.setCursor(0, y); y += 10;
    display.print(selectedField == ALARM_REPEAT_DAYS ? ">Days: " : " Days: ");
    for (int i = 0; i < 7; i++) {
      if (i == currentRepeatDayIndex && selectedField == ALARM_REPEAT_DAYS) display.print("[");
      display.print(weekDays[i][0]);
      display.print(a.repeatDays[i] ? "*" : " ");
      if (i == currentRepeatDayIndex && selectedField == ALARM_REPEAT_DAYS) display.print("]");
      display.print(" ");
    }
  }

  display.setCursor(0, y); y += 10;
  display.printf("%sEnabled: %s", selectedField == ALARM_ENABLED ? ">" : " ", a.enabled ? "Yes" : "No");

  display.setCursor(0, y);
  display.printf("%sMelody: %s", selectedField == ALARM_MELODY ? ">" : " ", melodyNames[a.melody]);
  display.display();
}

void checkAndTriggerAlarms() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  for (int i = 0; i < 3; i++) {
    Alarm &a = alarms[i];
    if (!a.enabled) continue;
    if (a.hour != timeinfo.tm_hour || a.minute != timeinfo.tm_min) continue;
    if (timeinfo.tm_min == lastTriggerMinute) continue;

    bool shouldTrigger = false;

    if (a.type == ONE_TIME) {
      shouldTrigger = true;
      a.enabled = false;
    } else if (a.type == SPECIFIC_DATE) {
      if (a.year == (timeinfo.tm_year + 1900) && a.month == (timeinfo.tm_mon + 1) && a.day == timeinfo.tm_mday)
        shouldTrigger = true;
    } else if (a.type == REPEATED) {
      int weekday = timeinfo.tm_wday == 0 ? 6 : timeinfo.tm_wday - 1;
      if (a.repeatDays[weekday]) shouldTrigger = true;
    }

    if (shouldTrigger) {
      alarmActive = true;
      lastTriggerMinute = timeinfo.tm_min;
      playMelody(a.melody, BUZZER_PIN);
      break;
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  dht.begin();
  connectToWiFi();
}

void loop() {
  unsigned long now = millis();
  checkAndTriggerAlarms();

  // MODE button: Short press to toggle screens / long press to clear repeat days
  bool modePressed = digitalRead(MODE_BUTTON_PIN) == LOW;

  if (modePressed && modeButtonPressTime == 0) {
    modeButtonPressTime = now;
  }

  if (!modePressed && modeButtonPressTime > 0) {
    unsigned long pressDuration = now - modeButtonPressTime;

    if (uiState == ALARM_CONFIG) {
      if (selectedField == ALARM_REPEAT_DAYS && pressDuration > 1000) {
        for (int i = 0; i < 7; i++) tempAlarm.repeatDays[i] = false;
      } else {
        do {
          selectedField = (AlarmField)((selectedField + 1) % 10);
        } while (!isFieldVisible(tempAlarm.type, selectedField));
      }
    } else {
      uiState = (uiState == IDLE_SCREEN) ? ALARM_OVERVIEW : IDLE_SCREEN;
    }

    lastInteractionTime = now;
    modeButtonPressTime = 0;
  }


  // ADJUST button
  if (digitalRead(ADJUST_BUTTON_PIN) == LOW && now - lastAdjustPress > 200) {
    lastAdjustPress = now;
    Alarm &a = tempAlarm;

    if (uiState == ALARM_OVERVIEW) {
      selectedAlarmIndex = (selectedAlarmIndex + 1) % 3;
    } else if (uiState == ALARM_CONFIG) {
      switch (selectedField) {
        case ALARM_TYPE: a.type = (AlarmType)((a.type + 1) % 3); break;
        case ALARM_TIME_HOUR: a.hour = (a.hour + 1) % 24; break;
        case ALARM_TIME_MIN: a.minute = (a.minute + 1) % 60; break;
        case ALARM_DATE_YEAR: a.year = (a.year < 2035) ? a.year + 1 : 2025; break;
        case ALARM_DATE_MONTH: a.month = (a.month % 12) + 1; break;
        case ALARM_DATE_DAY: a.day = (a.day % 31) + 1; break;
        case ALARM_REPEAT_DAYS: currentRepeatDayIndex = (currentRepeatDayIndex + 1) % 7; break;
        case ALARM_ENABLED: a.enabled = !a.enabled; break;
        case ALARM_MELODY: a.melody = (a.melody + 1) % 3; break;
        default: break;
      }
    }
    lastInteractionTime = now;
  }

  // CONFIRM button
  if (digitalRead(CONFIRM_BUTTON_PIN) == LOW && now - lastConfirmPress > 200) {
    lastConfirmPress = now;

    if (uiState == ALARM_OVERVIEW) {
      tempAlarm = alarms[selectedAlarmIndex];
      uiState = ALARM_CONFIG;
      selectedField = ALARM_TYPE;
    } else if (uiState == ALARM_CONFIG) {
      if (selectedField == ALARM_REPEAT_DAYS) {
        tempAlarm.repeatDays[currentRepeatDayIndex] = !tempAlarm.repeatDays[currentRepeatDayIndex];
      } else {
        alarms[selectedAlarmIndex] = tempAlarm;
        uiState = IDLE_SCREEN;
      }
    }
    lastInteractionTime = now;
  }

  // Timeout → return to idle
  if (now - lastInteractionTime > UI_TIMEOUT_MS && uiState != IDLE_SCREEN) {
    uiState = IDLE_SCREEN;
  }

  // Render current screen
  switch (uiState) {
    case IDLE_SCREEN: drawIdleScreen(); break;
    case ALARM_OVERVIEW: drawAlarmOverview(); break;
    case ALARM_CONFIG: drawAlarmConfig(); break;
  }

  delay(50);
}
