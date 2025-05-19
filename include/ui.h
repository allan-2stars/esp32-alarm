#ifndef UI_H
#define UI_H

#include "alarm.h"
#include "config.h"

enum UIState { IDLE_SCREEN, ALARM_OVERVIEW, ALARM_CONFIG };
extern UIState uiState;

void drawIdleScreen();
void drawAlarmOverview();
void drawAlarmConfig();

#endif
