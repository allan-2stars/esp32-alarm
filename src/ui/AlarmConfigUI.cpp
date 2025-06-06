#include "ui/AlarmConfigUI.h"
#include "services/AlarmStorageService.h"
#include "utils.h"
#include "globals.h"
#include "melodies.h"

extern const char* weekDays[];
extern AlarmStorageService alarmStorageService;
extern Alarm tempAlarm;
extern int currentRepeatDayIndex;

AlarmConfigUI::AlarmConfigUI(Adafruit_SSD1306 &display, Alarm* alarm, int index)
  : display(display), alarm(alarm), alarmIndex(index), selectedFieldIndex(ALARM_TYPE), 
  scrollOffset(0), done(false), currentRepeatDayIndex(0) {
  tempAlarm = *alarm;
}


void AlarmConfigUI::begin() {
  draw();
}

void AlarmConfigUI::update() {
  draw();
}

void AlarmConfigUI::nextField() {
  do {
    selectedFieldIndex = (AlarmField)((selectedFieldIndex + 1) % ALARM_SAVE_EXIT);
  } while (!isFieldVisible(tempAlarm.type, selectedFieldIndex));
  draw();
}

void AlarmConfigUI::prevField() {
  do {
    selectedFieldIndex = (AlarmField)((selectedFieldIndex - 1 + ALARM_SAVE_EXIT) % ALARM_SAVE_EXIT);
  } while (!isFieldVisible(tempAlarm.type, selectedFieldIndex));
  draw();
}

void AlarmConfigUI::adjustValue(bool increase) {
  int delta = increase ? 1 : -1;
  switch (selectedFieldIndex) {
    case ALARM_TYPE:
      tempAlarm.type = (AlarmType)((tempAlarm.type + delta + 3) % 3);
      break;
    case ALARM_TIME_HOUR:
      tempAlarm.hour = (tempAlarm.hour + delta + 24) % 24;
      break;
    case ALARM_TIME_MIN:
      tempAlarm.minute = (tempAlarm.minute + delta + 60) % 60;
      break;
    case ALARM_DATE_YEAR: {
      int currentYear = getCurrentYear();
      int maxYear = currentYear + 10;
      tempAlarm.year += delta;
      if (tempAlarm.year < currentYear) tempAlarm.year = maxYear;
      if (tempAlarm.year > maxYear) tempAlarm.year = currentYear;
      break;
    }
    case ALARM_DATE_MONTH:
      tempAlarm.month = (tempAlarm.month + delta - 1 + 12) % 12 + 1;
      break;
    case ALARM_DATE_DAY: {
      int maxDay = getMaxDay(tempAlarm.year, tempAlarm.month);
      tempAlarm.day = (tempAlarm.day + delta - 1 + maxDay) % maxDay + 1;
      break;
    }
    case ALARM_REPEAT_DAYS:
      currentRepeatDayIndex = (currentRepeatDayIndex + delta + 7) % 7;
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
  draw();
}

void AlarmConfigUI::confirm() {
    if (selectedFieldIndex == ALARM_REPEAT_DAYS) {
        tempAlarm.repeatDays[currentRepeatDayIndex] = !tempAlarm.repeatDays[currentRepeatDayIndex];
    } else {
        *alarm = tempAlarm;
        alarm->version = SCREEN_ALARM_VERSION;
        alarmStorageService.saveAlarm(*alarm, alarmIndex);
        done = true;
    }
    draw();
}


bool AlarmConfigUI::isDone() const {
  return done;
}

AlarmField AlarmConfigUI::getSelectedField() const {
  return selectedFieldIndex;
}

int AlarmConfigUI::getSelectedMelody() const {
  return tempAlarm.melody;
}

void AlarmConfigUI::draw() {
  display.clearDisplay();
  std::vector<AlarmField> visibleFields;
  if (isFieldVisible(tempAlarm.type, ALARM_TYPE)) visibleFields.push_back(ALARM_TYPE);
  visibleFields.push_back(ALARM_TIME_HOUR);
  if (isFieldVisible(tempAlarm.type, ALARM_DATE_YEAR)) visibleFields.push_back(ALARM_DATE_YEAR);
  if (isFieldVisible(tempAlarm.type, ALARM_REPEAT_DAYS)) visibleFields.push_back(ALARM_REPEAT_DAYS);
  visibleFields.push_back(ALARM_ENABLED);
  visibleFields.push_back(ALARM_MELODY);

  int totalFields = visibleFields.size();
  const int maxVisibleLines = SCREEN_HEIGHT / 10 - 2;
  int selectedIndex = 0;
  for (int i = 0; i < totalFields; ++i) {
    if (visibleFields[i] == selectedFieldIndex) selectedIndex = i;
  }
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
        display.printf("%sType: %s", selectedFieldIndex == ALARM_TYPE ? ">" : " ",
          tempAlarm.type == ONE_TIME ? "Once" :
          tempAlarm.type == SPECIFIC_DATE ? "Date" : "Repeat");
        break;

      case ALARM_TIME_HOUR:
        display.printf(" Time: %s%02d%s:%s%02d%s",
          selectedFieldIndex == ALARM_TIME_HOUR ? "[" : "", tempAlarm.hour, selectedFieldIndex == ALARM_TIME_HOUR ? "]" : "",
          selectedFieldIndex == ALARM_TIME_MIN ? "[" : "", tempAlarm.minute, selectedFieldIndex == ALARM_TIME_MIN ? "]" : "");
        break;

      case ALARM_DATE_YEAR:
        display.printf(" Date: %s%04d%s-%s%02d%s-%s%02d%s",
          selectedFieldIndex == ALARM_DATE_YEAR ? "[" : "", tempAlarm.year, selectedFieldIndex == ALARM_DATE_YEAR ? "]" : "",
          selectedFieldIndex == ALARM_DATE_MONTH ? "[" : "", tempAlarm.month, selectedFieldIndex == ALARM_DATE_MONTH ? "]" : "",
          selectedFieldIndex == ALARM_DATE_DAY ? "[" : "", tempAlarm.day, selectedFieldIndex == ALARM_DATE_DAY ? "]" : "");
        break;

      case ALARM_REPEAT_DAYS: {
        display.printf("%sDays:", selectedFieldIndex == ALARM_REPEAT_DAYS ? ">" : " ");
        drawLine++;
        y = HEADER_HEIGHT + drawLine * 10;
        if (y < SCREEN_HEIGHT - 8) {
          display.setCursor(0, y);
          for (int j = 0; j < 7; j++) {
            if (j == currentRepeatDayIndex && selectedFieldIndex == ALARM_REPEAT_DAYS) display.print("[");
            display.print(weekDays[j][0]);
            display.print(tempAlarm.repeatDays[j] ? "*" : " ");
            if (j == currentRepeatDayIndex && selectedFieldIndex == ALARM_REPEAT_DAYS) display.print("]");
            display.print(" ");
          }
        }
        break;
      }

      case ALARM_ENABLED:
        display.printf("%sEnabled: %s", selectedFieldIndex == ALARM_ENABLED ? ">" : " ",
          tempAlarm.enabled ? "Yes" : "No");
        break;

      case ALARM_MELODY:
        display.printf("%sMelody: %s", selectedFieldIndex == ALARM_MELODY ? ">" : " ",
          melodyNames[tempAlarm.melody]);
        break;

      default:
        break;
    }
    drawLine++;
  }

  if (scrollOffset + maxVisibleLines < totalFields) {
    display.setCursor(SCREEN_WIDTH - 6, SCREEN_HEIGHT - 8);
    display.print("~");
  }


  display.display();
}

void AlarmConfigUI::setSelectedMelody(int index) {
  tempAlarm.melody = index;
  draw();
}

