#pragma once

#include <Adafruit_SSD1306.h>
#include <vector>
#include "alarm.h"
#include "config.h"
#include "ui.h"

// Max number of visible lines on screen at once
#define MAX_VISIBLE_LINES 4

class AlarmConfigUI {
  public:
    AlarmConfigUI(Adafruit_SSD1306 &display, Alarm *alarm);

    void begin();                     // Call once to start editing
    void update();                    // Redraw the screen
    void nextField();                // Move to next editable field
    void prevField();                // Move to previous editable field
    void adjustValue(bool increase); // Change value of selected field
    void confirm();                  // Save changes to original alarm
    void resetCursor();              // Reset field selection to top

    bool isDone() const;

  private:
    Adafruit_SSD1306 &display;
    Alarm *alarm;
    Alarm tempAlarm;
    int selectedFieldIndex;
    std::vector<AlarmField> visibleFields;
    int scrollOffset;
    bool done;
    int currentRepeatDayIndex = 0;


    void determineVisibleFields();
    void draw();
    void drawField(int y, AlarmField field, bool selected);
    String getFieldLabel(AlarmField field);
    String getFieldValue(AlarmField field);
    void applyAdjustment(AlarmField field, bool increase);
};
