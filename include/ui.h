#ifndef UI_H
#define UI_H
#pragma once
//#include "alarm.h"

enum UIState {
  IDLE_SCREEN,
  ALARM_OVERVIEW,
  ALARM_CONFIG,
  MELODY_PREVIEW,
  ALARM_RINGING,
  ALARM_SNOOZE_MESSAGE,
  ERROR_SCREEN,
  MESSAGE_DISPLAY, // New Message display screen
};

extern UIState uiState;

void drawIdleScreen();
void drawAlarmOverview();
void drawAlarmConfig();
void drawMelodyPreview(int selectedIndex);
void drawSnoozeMessage(bool wasSnoozed);
void drawErrorScreen();

#include <Adafruit_SSD1306.h>
void initNTP();
void initDisplay(Adafruit_SSD1306 &display);
void checkIdleAndSleep();

#endif
