#include "ui/AlarmOverviewUI.h"
#include "melodies.h"
#include "utils.h"

AlarmOverviewUI::AlarmOverviewUI(Adafruit_SSD1306 &display) : display(display) {}

void AlarmOverviewUI::draw(const Alarm* alarms, int selectedIndex) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(TEXT_COLOR);
  display.setCursor(0, 0);
  display.print("Alarm Overview:");

  for (int i = 0; i < MAX_SCREEN_ALARMS; ++i) {
    int yPos = HEADER_HEIGHT + i * 12; 
    display.setCursor(0, yPos);

    // Indicator for selection
    if (i == selectedIndex) {
      display.print("> ");
    } else {
      display.print("  ");
    }

    // Format: [On/Off] Time - Type (Melody)
    if (!alarms[i].enabled) {
      display.print("[ ] --:-- ");
    } else {
      char timeStr[6];
      sprintf(timeStr, "%02d:%02d", alarms[i].hour, alarms[i].minute);
      display.print("[x] ");
      display.print(timeStr);
      display.print(" ");
    }

    display.print(getAlarmTypeLabel(alarms[i].type));
    // display.print(" (");
    // display.print(getMelodyName(alarms[i].melody));
    // display.println(")");
  }

  display.setCursor(0, SCREEN_HEIGHT - 10);
  display.print("Mod:Back  Cmf:Edit");
  display.display();
}
