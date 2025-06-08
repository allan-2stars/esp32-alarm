#include "ui/UIManager.h"
#include "services/MelodyService.h"
#include "services/AlarmStorageService.h"
#include "services/LedService.h"
#include "utils.h"
#include "globals.h"
#include "melodies.h"
#include "buttons.h"

extern MelodyService melodyService;
extern AlarmStorageService alarmStorageService;
extern LedService ledService;
extern Alarm alarms[MAX_SCREEN_ALARMS];
extern Alarm tempAlarm;
extern AlarmPlayerService alarmPlayerService;

unsigned long temporaryScreenStart = 0;
unsigned long temporaryScreenDuration = 0;


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
      snoozeUntil(0),
      messageDisplayStart(0) {}
      

void UIManager::begin() {
  idleUI.update();
}

void UIManager::showMessage(const String& msg, UIState returnTo, unsigned long durationMs) {
    temporaryMessage = msg;
    temporaryScreenStart = millis();
    temporaryScreenDuration = durationMs;
    returnState = returnTo;
    messageDisplayUI.show(msg);
    switchTo(MESSAGE_DISPLAY);
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
        showMessage(" Alarm Saved", ALARM_OVERVIEW, 2000);
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
    case MESSAGE_DISPLAY:
      messageDisplayUI.render();
      if (temporaryScreenDuration > 0 &&
          millis() - temporaryScreenStart > temporaryScreenDuration) {
          switchTo(returnState);
      }
      break;
    case ROBOT_FACE_DISPLAY:
    
      robotFaceUI.update(); // new robot render.
      
      break;
    default:
      break;
    }
  }

void UIManager::handleMode() {
  recordInteraction();
  melodyService.stop();
  ledService.stopAlarmLights();
  lastInteraction = millis();

  // Prioritize MESSAGE_DISPLAY scroll behavior
  if (uiState == MESSAGE_DISPLAY && temporaryScreenDuration == 0) {
    messageDisplayUI.scrollDown();
    return;
  }

  switch (currentState) {
    case ALARM_CONFIG:
      if (alarmConfigUI) {
        alarmConfigUI->nextField();
      }
      break;

    case MELODY_PREVIEW:
      currentState = ALARM_CONFIG;
      break;

    case ALARM_RINGING:
      snoozeUntil = time(nullptr) + 600;
      lastSnoozed = true;
      showMessage(" Snoozed\n for 10 mins", IDLE_SCREEN, 2000);
      break;
    case ROBOT_FACE_DISPLAY:
      return;
    default:
      // Toggle between IDLE and OVERVIEW
      switchTo(currentState == IDLE_SCREEN ? ALARM_OVERVIEW : IDLE_SCREEN);
      break;
  }
}


void UIManager::handleAdjust() {
  recordInteraction();
  melodyService.stop();
  ledService.stopAlarmLights();
  lastInteraction = millis();

    // Prioritize MESSAGE_DISPLAY scroll behavior
  if (uiState == MESSAGE_DISPLAY && temporaryScreenDuration == 0) {
    messageDisplayUI.scrollUp();
    return;
  }
  switch (currentState) {
    case ALARM_OVERVIEW:
      selectedAlarmIndex = (selectedAlarmIndex + 1) % MAX_SCREEN_ALARMS;
      break;

    case ALARM_CONFIG:
      if (alarmConfigUI) {
        if (alarmConfigUI->getSelectedField() == ALARM_MELODY) {
          currentState = MELODY_PREVIEW;
          tempAlarm.melody = alarms[selectedAlarmIndex].melody;
          previewMelodyIndex = tempAlarm.melody;  // ✅ Fix: keep UI in sync
          alarmPlayerService.playAlarm(tempAlarm, false, false);  // no loop, no lights
        } else {
          alarmConfigUI->adjustValue(true);
        }
      }
      break;

    case MELODY_PREVIEW:
      tempAlarm.melody = (tempAlarm.melody + 1) % MELODY_COUNT;
      previewMelodyIndex = tempAlarm.melody;
      alarmPlayerService.playAlarm(tempAlarm, false, false);
      break;

    case ALARM_RINGING:
      snoozeUntil = time(nullptr) + 600;
      lastSnoozed = true;
      showMessage(" Snoozed\n for 10 mins", IDLE_SCREEN, 3000);
      break;
    case ROBOT_FACE_DISPLAY:
      break;

    default:
      break;
  }
}


void UIManager::handleConfirm() {
  recordInteraction();
  melodyService.stop();
  ledService.stopAlarmLights();
  lastInteraction = millis();

  // Prioritize MESSAGE_DISPLAY confirm behavior
  if (uiState == MESSAGE_DISPLAY) {
    switchTo(returnState);
    return;
  }

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
          showMessage(" Alarm Set!", ALARM_OVERVIEW, 2000);
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
      showMessage(" Alarm Stopped", IDLE_SCREEN, 3000);
      break;
    case ROBOT_FACE_DISPLAY:
      break;
    default:
    break;
  }
}

void UIManager::switchTo(UIState newState) {
  if (currentState == newState) return;
  currentState = newState;
  lastInteraction = millis();
  resetAllButtons();

  //resetAdjustRepeat();  // 🔄 Reset held state on UI transition
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
    case MESSAGE_DISPLAY:
      // Just allow rendering via uiManager.update()
      break;
    case ROBOT_FACE_DISPLAY:
      robotFaceUI.begin();
      break;
    default:
      break;
  }
}



UIState UIManager::getCurrentState() const {
  return currentState;
}