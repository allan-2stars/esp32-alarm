#ifndef UI_H
#define UI_H

#include "alarm.h"

enum UIState {
  IDLE_SCREEN,
  ALARM_OVERVIEW,
  ALARM_CONFIG,
  MELODY_PREVIEW,
  ALARM_RINGING,  // New Ringing Screen
  ALARM_SNOOZE_MESSAGE
};

extern UIState uiState;

void drawIdleScreen();
void drawAlarmOverview();
void drawAlarmConfig();
void drawMelodyPreview(int selectedIndex);
void drawSnoozeMessage(bool wasSnoozed);

#endif
