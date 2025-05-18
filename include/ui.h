#ifndef UI_H
#define UI_H

#include "alarm.h"

#define UI_TIMEOUT_MS 60000

enum UIState { IDLE_SCREEN, ALARM_OVERVIEW, ALARM_CONFIG };
extern UIState uiState;

void drawIdleScreen();
void drawAlarmOverview();
void drawAlarmConfig();

#endif
