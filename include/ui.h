#ifndef UI_H
#define UI_H

#include "alarm.h"

enum UIState {
  IDLE_SCREEN,
  ALARM_OVERVIEW,
  ALARM_CONFIG,
  MELODY_PREVIEW,
  ALARM_RINGING,
  ALARM_SNOOZE_MESSAGE,
  ERROR_SCREEN, // New Error Screen
};

extern UIState uiState;

void drawIdleScreen();
void drawAlarmOverview();
void drawAlarmConfig();
void drawMelodyPreview(int selectedIndex);
void drawSnoozeMessage(bool wasSnoozed);
void drawErrorScreen();

#endif
