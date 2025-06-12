#pragma once

#include <Adafruit_SSD1306.h>
#include "alarm.h"
#include "config.h"
#include "utils.h"

class AlarmConfigUI {
  public:
    AlarmConfigUI(Adafruit_SSD1306 &display, Alarm *alarm, int index);

    void begin();
    void update();
    void nextField();
    void prevField();
    void adjustValue(bool increase);
    void confirm();
    bool isDone() const;
    AlarmField getSelectedField() const;
    int getSelectedMelody() const;
    void setSelectedMelody(int index);

  private:
    Adafruit_SSD1306 &display;
    Alarm *alarm;
    int alarmIndex;

    Alarm tempAlarm;
    AlarmField selectedFieldIndex;
    int scrollOffset;
    bool done;
    int currentRepeatDayIndex;
    const char* weekDays[7] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

    void draw();
    String getFieldLabel(AlarmField field);
    String getFieldValue(AlarmField field);
};
