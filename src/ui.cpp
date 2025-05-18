#include <Adafruit_SSD1306.h>
#include "ui.h"
#include "alarm.h"

extern Adafruit_SSD1306 display;
extern Alarm alarms[3];
extern Alarm tempAlarm;
extern int selectedAlarmIndex;
extern AlarmField selectedField;
extern int currentRepeatDayIndex;
extern UIState uiState;

const char* melodyNames[] = {"Wish", "White", "Jingle", "Rudolf", "Santa", "Silent"};
const char* weekDays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

#include <time.h>

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
  display.printf("T:%.1fC H:%.1f%%", 24.0, 50.0); // Placeholder for actual sensor values

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
  //Alarm &a = alarms[selectedAlarmIndex];
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
  display.printf(" Time: %s%02d%s:%s%02d%s",
    selectedField == ALARM_TIME_HOUR ? "[" : "", a.hour, selectedField == ALARM_TIME_HOUR ? "]" : "",
    selectedField == ALARM_TIME_MIN ? "[" : "", a.minute, selectedField == ALARM_TIME_MIN ? "]" : "");

  if (isFieldVisible(a.type, ALARM_DATE_YEAR)) {
    display.setCursor(0, y); y += 10;
    display.printf(" Date: %s%04d%s-%s%02d%s-%s%02d%s",
      selectedField == ALARM_DATE_YEAR ? "[" : "", a.year, selectedField == ALARM_DATE_YEAR ? "]" : "",
      selectedField == ALARM_DATE_MONTH ? "[" : "", a.month, selectedField == ALARM_DATE_MONTH ? "]" : "",
      selectedField == ALARM_DATE_DAY ? "[" : "", a.day, selectedField == ALARM_DATE_DAY ? "]" : "");
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
