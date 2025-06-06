// include/ui/UIManager.h
#pragma once

#include "ui/IdleUI.h"
#include "ui/AlarmOverviewUI.h"
#include "ui/AlarmConfigUI.h"
#include "ui/MelodyPreviewUI.h"
#include "ui/BellUI.h"
#include "ui/SunMoonUI.h"
#include "alarm.h"
#include "config.h"
//#include "globals.h"

class UIManager {
public:
  UIManager(Adafruit_SSD1306 &display);

  void begin();
  void update();
  void handleMode();
  void handleAdjust();
  void handleConfirm();
  void switchTo(UIState state);
  UIState getCurrentState() const;
  // to show some temp message on the screen
  void showMessageAndReturn(const String& message, UIState nextScreen, unsigned long durationMs = 3000);
  void showMessage(const String& msg);

private:
  Adafruit_SSD1306 &display;
  IdleUI idleUI;
  AlarmOverviewUI overviewUI;
  MelodyPreviewUI melodyUI;
  BellUI bellUI;
  AlarmConfigUI* alarmConfigUI;
  unsigned long lastInteraction = 0;
  // after the temp screem return to Idle screen
  UIState returnState = IDLE_SCREEN;



  UIState currentState;
  int selectedAlarmIndex;
  int previewMelodyIndex;
  bool alarmActive;
  time_t snoozeUntil;
  unsigned long messageDisplayStart;
  void confirmAlarmSelection();
  // send temp message on screen
  String temporaryMessage = "";

};

