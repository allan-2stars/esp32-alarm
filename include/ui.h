
#pragma once
//#include "alarm.h"

enum UIState {
  IDLE_SCREEN,
  ALARM_OVERVIEW,
  ALARM_CONFIG,
  MELODY_PREVIEW,
  ALARM_RINGING,
  ERROR_SCREEN,
  MESSAGE_DISPLAY, // New Message display screen
  SCROLLABLE_MESSAGE // New screen
};

extern UIState uiState;

void drawIdleScreen();
void drawAlarmOverview();
void drawAlarmConfig();
void drawMelodyPreview(int selectedIndex);
void drawSnoozeMessage(bool wasSnoozed);
void drawSaveAlarmMessage();
void drawErrorScreen();

#include <Adafruit_SSD1306.h>
void initNTP();
void initDisplay(Adafruit_SSD1306 &display);
void checkIdleAndSleep();
