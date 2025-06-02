#include "ui/AlarmConfigUI.h"
#include "utils.h"
#include "melodies.h"
#include <Arduino.h>

extern int selectedAlarmIndex;

AlarmConfigUI::AlarmConfigUI(Adafruit_SSD1306 &display, Alarm *alarm)
  : display(display), alarm(alarm), selectedFieldIndex(0), scrollOffset(0), done(false), currentRepeatDayIndex(0) {
  tempAlarm = *alarm;
}

void AlarmConfigUI::begin() {
  determineVisibleFields();
  draw();
}

void AlarmConfigUI::update() {
  draw();
}

void AlarmConfigUI::nextField() {
  selectedFieldIndex = (selectedFieldIndex + 1) % visibleFields.size();  // Wrap around
  scrollOffset = adjustVisibleStart(selectedFieldIndex, scrollOffset, MAX_VISIBLE_LINES, visibleFields.size());

  draw();
}


void AlarmConfigUI::prevField() {
  selectedFieldIndex--;
  if (selectedFieldIndex < 0) selectedFieldIndex = visibleFields.size() - 1;  // Wrap around to bottom
  scrollOffset = adjustVisibleStart(selectedFieldIndex, scrollOffset, MAX_VISIBLE_LINES, visibleFields.size());
  draw();
}

void AlarmConfigUI::adjustValue(bool increase) {
  if (selectedFieldIndex < 0 || selectedFieldIndex >= (int)visibleFields.size()) return;
  applyAdjustment(visibleFields[selectedFieldIndex], increase);
  draw();
}

void AlarmConfigUI::confirm() {
  *alarm = tempAlarm;
  done = true;
}

bool AlarmConfigUI::isDone() const {
  return done;
}

void AlarmConfigUI::resetCursor() {
  selectedFieldIndex = 0;
  scrollOffset = 0;
  draw();
}

void AlarmConfigUI::determineVisibleFields() {
  visibleFields.clear();
  if (isFieldVisible(tempAlarm.type, ALARM_TYPE)) visibleFields.push_back(ALARM_TYPE);
  visibleFields.push_back(ALARM_TIME_HOUR);
  visibleFields.push_back(ALARM_TIME_MIN);
  if (isFieldVisible(tempAlarm.type, ALARM_DATE_YEAR)) {
    visibleFields.push_back(ALARM_DATE_YEAR);
    visibleFields.push_back(ALARM_DATE_MONTH);
    visibleFields.push_back(ALARM_DATE_DAY);
  }
  if (isFieldVisible(tempAlarm.type, ALARM_REPEAT_DAYS)) visibleFields.push_back(ALARM_REPEAT_DAYS);
  visibleFields.push_back(ALARM_ENABLED);
  visibleFields.push_back(ALARM_MELODY);
  visibleFields.push_back(ALARM_SAVE_EXIT);
}

void AlarmConfigUI::draw() {
  display.clearDisplay();
  display.setTextColor(TEXT_COLOR);
  display.setTextSize(1);
  int drawLine = 0;

  display.setTextColor(TEXT_COLOR);
  display.setCursor(0, 0);
  display.print("Config Alarm ");
  display.print(selectedAlarmIndex + 1);  // Show as 1-based

  //

  for (size_t i = 0; i < visibleFields.size(); ++i) {
    if (i < scrollOffset || i >= scrollOffset + MAX_VISIBLE_LINES) continue;

    AlarmField field = visibleFields[i];
    bool selectedHour = (i == selectedFieldIndex);
    bool selectedMin = (i + 1 == selectedFieldIndex);

    if (field == ALARM_TIME_HOUR && i + 1 < visibleFields.size() && visibleFields[i + 1] == ALARM_TIME_MIN) {
      String label = "Time:";

      String hourStr = (tempAlarm.hour < 10 ? "0" : "") + String(tempAlarm.hour);
      String minStr = (tempAlarm.minute < 10 ? "0" : "") + String(tempAlarm.minute);

      if (selectedHour) hourStr = ">" + hourStr + "<";
      if (selectedMin)  minStr  = ">" + minStr + "<";

      String value = hourStr + ":" + minStr;

      if (selectedHour || selectedMin) {
        display.fillRect(0, HEADER_HEIGHT + drawLine * 10, SCREEN_WIDTH, 10, SSD1306_WHITE);
        display.setTextColor(SSD1306_BLACK);
      } else {
        display.setTextColor(TEXT_COLOR);
      }

      display.setCursor(0, HEADER_HEIGHT + drawLine * 10);
      display.print(label);
      display.setCursor(SCREEN_WIDTH / 2, HEADER_HEIGHT + drawLine * 10);
      display.print(value);

      drawLine++;
      i++;  // skip ALARM_TIME_MIN
      continue;
    }

    bool selected = (i == selectedFieldIndex);
    drawField(HEADER_HEIGHT + drawLine * 10, field, selected);
    drawLine++;
  }

  display.display();
}

void AlarmConfigUI::drawField(int y, AlarmField field, bool selected) {
  String label = getFieldLabel(field);
  String value = getFieldValue(field);
  if (selected) {
    display.fillRect(0, y, SCREEN_WIDTH, 10, SSD1306_WHITE);
    display.setTextColor(SSD1306_BLACK);
  } else {
    display.setTextColor(TEXT_COLOR);
  }
  display.setCursor(0, y);
  display.print(label);
  display.setCursor(SCREEN_WIDTH / 2, y);
  display.print(value);
}

String AlarmConfigUI::getFieldLabel(AlarmField field) {
  switch (field) {
    case ALARM_TYPE: return "Type:";
    case ALARM_TIME_HOUR: return "Time:";
    case ALARM_DATE_YEAR: return "Year:";
    case ALARM_DATE_MONTH: return "Month:";
    case ALARM_DATE_DAY: return "Day:";
    case ALARM_REPEAT_DAYS: return "Repeat:";
    case ALARM_ENABLED: return "On/Off:";
    case ALARM_MELODY: return "Melody:";
    case ALARM_SAVE_EXIT: return "Save:";
    default: return "";
  }
}

String AlarmConfigUI::getFieldValue(AlarmField field) {
  switch (field) {
    case ALARM_TYPE:
      switch (tempAlarm.type) {
        case ONE_TIME: return "One-Time";
        case SPECIFIC_DATE: return "Date";
        case REPEATED: return "Repeat";
        default: return "Unknown";
      }

    case ALARM_DATE_YEAR: return String(tempAlarm.year);
    case ALARM_DATE_MONTH: return String(tempAlarm.month);
    case ALARM_DATE_DAY: return String(tempAlarm.day);
    case ALARM_REPEAT_DAYS: return getRepeatDaysString(tempAlarm.repeatDays);
    case ALARM_ENABLED: return tempAlarm.enabled ? "ON" : "OFF";
    case ALARM_MELODY: return getMelodyName(tempAlarm.melody);
    case ALARM_SAVE_EXIT: return "OK";
    default: return "";
  }
}

void AlarmConfigUI::applyAdjustment(AlarmField field, bool increase) {
  int delta = increase ? 1 : -1;
  switch (field) {
    case ALARM_TYPE:
      tempAlarm.type = (AlarmType)((tempAlarm.type + delta + 3) % 3);
      determineVisibleFields();
      resetCursor();
      break;
    case ALARM_TIME_HOUR:
      tempAlarm.hour = (tempAlarm.hour + delta + 24) % 24;
      break;
    case ALARM_TIME_MIN:
      tempAlarm.minute = (tempAlarm.minute + delta + 60) % 60;
      break;
    case ALARM_DATE_YEAR:
      tempAlarm.year = constrain(tempAlarm.year + delta, 2024, 2100);
      break;
    case ALARM_DATE_MONTH:
      tempAlarm.month = constrain(tempAlarm.month + delta, 1, 12);
      break;
    case ALARM_DATE_DAY:
      tempAlarm.day = constrain(tempAlarm.day + delta, 1, 31);
      break;
    case ALARM_REPEAT_DAYS:
      currentRepeatDayIndex = (currentRepeatDayIndex + 1) % 7;
      break;
    case ALARM_ENABLED:
      tempAlarm.enabled = !tempAlarm.enabled;
      break;
    case ALARM_MELODY:
      tempAlarm.melody = (tempAlarm.melody + delta + MELODY_COUNT) % MELODY_COUNT;
      break;
    default:
      break;
  }
}
