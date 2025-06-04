#include "ui/UIManager.h"
#include "services/MelodyService.h"
#include "services/AlarmStorageService.h"
#include "services/LedService.h"
#include "utils.h"
#include "globals.h"
#include "melodies.h"

extern MelodyService melodyService;
extern AlarmStorageService alarmStorageService;
extern LedService ledService;
extern Alarm alarms[MAX_SCREEN_ALARMS];
extern Alarm tempAlarm;

UIManager::UIManager(Adafruit_SSD1306 &display)
    : display(display),
      idleUI(display),
      overviewUI(display),
      melodyUI(display),
      bellUI(display),
      alarmConfigUI(nullptr),
      currentState(IDLE_SCREEN),
      selectedAlarmIndex(0),
      previewMelodyIndex(0),
      alarmActive(false),
      snoozeUntil(0),
      messageDisplayStart(0) {}

void UIManager::begin() {
  idleUI.update();
}

void UIManager::update() {
  switch (currentState) {
    case IDLE_SCREEN:
      idleUI.update();
      break;
    case ALARM_OVERVIEW:
      overviewUI.draw(alarms, selectedAlarmIndex);
      break;
    case ALARM_CONFIG:
      if (!alarmConfigUI) {
        alarmConfigUI = new AlarmConfigUI(display, &alarms[selectedAlarmIndex], selectedAlarmIndex);
        alarmConfigUI->begin();
      }
      if (alarmConfigUI->isDone()) {
        delete alarmConfigUI;
        alarmConfigUI = nullptr;
        switchTo(ALARM_SAVE_MESSAGE);
      } else {
        alarmConfigUI->update();
      }
      break;
    case MELODY_PREVIEW:
      melodyUI.draw(previewMelodyIndex);
      break;
    case ALARM_RINGING:
      bellUI.update();
      ledService.updateAlarmLights();
      break;
    case ALARM_SAVE_MESSAGE:
      drawSaveAlarmMessage();
      // add 2 seconds delay to give the visual clue
      if (millis() - messageDisplayStart > 2000) {
        switchTo(IDLE_SCREEN);
      }
      break;
    case ALARM_SNOOZE_MESSAGE:
      drawSnoozeMessage(lastSnoozed);
      if (millis() - messageDisplayStart > 2000) {
        switchTo(IDLE_SCREEN);
      }
      break;
    case ERROR_SCREEN:
      drawErrorScreen();
      break;
  }
}

void UIManager::handleMode() {
  recordInteraction();
  melodyService.stop();
  ledService.stopAlarmLights();
  lastInteraction = millis();


  if (currentState == ALARM_CONFIG && alarmConfigUI) {
    alarmConfigUI->nextField();
  } else if (currentState == MELODY_PREVIEW) {
    currentState = ALARM_CONFIG;
  } else if (currentState == ALARM_RINGING) {
    snoozeUntil = time(nullptr) + 600;
    lastSnoozed = true;
    currentState = ALARM_SNOOZE_MESSAGE;
    messageDisplayStart = millis();
  } else {
    switchTo(currentState == IDLE_SCREEN ? ALARM_OVERVIEW : IDLE_SCREEN);
  }
}

void UIManager::handleAdjust() {
  recordInteraction();
  melodyService.stop();
  ledService.stopAlarmLights();
  lastInteraction = millis();


  if (currentState == ALARM_OVERVIEW) {
    selectedAlarmIndex = (selectedAlarmIndex + 1) % MAX_SCREEN_ALARMS;
  } else if (currentState == ALARM_CONFIG && alarmConfigUI) {
    if (alarmConfigUI->getSelectedField() == ALARM_MELODY) {
      currentState = MELODY_PREVIEW;
      previewMelodyIndex = alarms[selectedAlarmIndex].melody;
      melodyService.play(getMelodyData(previewMelodyIndex),
                         getMelodyLength(previewMelodyIndex),
                         getMelodyTempo(previewMelodyIndex),
                         BUZZER_PIN, false);
    } else {
      alarmConfigUI->adjustValue(true);
    }
  } else if (currentState == MELODY_PREVIEW) {
    previewMelodyIndex = (previewMelodyIndex + 1) % MELODY_COUNT;
    melodyService.play(getMelodyData(previewMelodyIndex),
                       getMelodyLength(previewMelodyIndex),
                       getMelodyTempo(previewMelodyIndex),
                       BUZZER_PIN, false);
  } else if (currentState == ALARM_RINGING) {
    snoozeUntil = time(nullptr) + 600;
    lastSnoozed = true;
    currentState = ALARM_SNOOZE_MESSAGE;
    messageDisplayStart = millis();
  }
}

void UIManager::handleConfirm() {
  recordInteraction();
  melodyService.stop();
  ledService.stopAlarmLights();
  lastInteraction = millis();

  switch (currentState) {
    case ALARM_OVERVIEW:
      tempAlarm = alarms[selectedAlarmIndex];
      if ((tempAlarm.year == 0 || tempAlarm.month == 0 || tempAlarm.day == 0) && isTimeAvailable()) {
        setAlarmToCurrentTime(tempAlarm);
      }
      alarmConfigUI = new AlarmConfigUI(display, &alarms[selectedAlarmIndex], selectedAlarmIndex);
      alarmConfigUI->begin();
      switchTo(ALARM_CONFIG);
      break;

    case ALARM_CONFIG:
      if (alarmConfigUI) {
        alarmConfigUI->confirm();
        if (alarmConfigUI->isDone()) {
          delete alarmConfigUI;
          alarmConfigUI = nullptr;
          messageDisplayStart = millis();
          switchTo(ALARM_SAVE_MESSAGE);
        }
      }
      break;

    case MELODY_PREVIEW:
      tempAlarm.melody = previewMelodyIndex;
      if (alarmConfigUI) {
        alarmConfigUI->setSelectedMelody(previewMelodyIndex);
      }
      switchTo(ALARM_CONFIG);
      break;

    case ALARM_RINGING:
      lastSnoozed = false;
      messageDisplayStart = millis();
      switchTo(ALARM_SNOOZE_MESSAGE);
      break;

    default:
      break;
  }
}


void UIManager::switchTo(UIState newState) {
  currentState = newState;
  lastInteraction = millis();  // reset timeout
  Serial.printf("Switching UI to: %d\n", newState);

  switch (newState) {
    case IDLE_SCREEN:
      idleUI.update();
      break;
    case ALARM_OVERVIEW:
      overviewUI.draw(alarms, selectedAlarmIndex);
      break;
    case MELODY_PREVIEW:
      melodyUI.draw(previewMelodyIndex);
      break;
    case ALARM_RINGING:
      bellUI.update();
      break;
    case ALARM_CONFIG:
      if (alarmConfigUI) alarmConfigUI->update();
      break;
    case ALARM_SNOOZE_MESSAGE:
      bellUI.update();
      break;
    case MESSAGE_DISPLAY:
      // optional: handle message state
      break;
  }
}


UIState UIManager::getCurrentState() const {
  return currentState;
}

