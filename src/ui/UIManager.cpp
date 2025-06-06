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

void UIManager::showMessage(const String& msg) {
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
        showMessageAndReturn("✅ Alarm Saved", ALARM_OVERVIEW);
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
      // Optional auto-return timeout:
      if (temporaryScreenDuration > 0 && millis() - temporaryScreenStart > temporaryScreenDuration) {
        switchTo(returnState);
      }
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
    showMessageAndReturn(" Snoozed\n for 10 mins", IDLE_SCREEN, 3000);
  }
  if (uiState == MESSAGE_DISPLAY) {
    messageDisplayUI.scrollDown();
  }
  
  else {
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
      tempAlarm.melody = alarms[selectedAlarmIndex].melody;
      previewMelodyIndex = tempAlarm.melody;  // ✅ Fix: keep UI in sync
      //if in melody review scree, no loop melody, and no light up
      alarmPlayerService.playAlarm(tempAlarm, false, false);/////

    } else {
      alarmConfigUI->adjustValue(true);
    }
  } else if (currentState == MELODY_PREVIEW) {
    tempAlarm.melody = (tempAlarm.melody + 1) % MELODY_COUNT;
    previewMelodyIndex = tempAlarm.melody;  // ✅ Fix: keep UI in sync
    Serial.println("preview Melody index: ");
    Serial.print(previewMelodyIndex);
    alarmPlayerService.playAlarm(tempAlarm, false, false);

  } else if (currentState == ALARM_RINGING) {
    snoozeUntil = time(nullptr) + 600;
    lastSnoozed = true;
    showMessageAndReturn(" Snoozed\n for 10 mins", IDLE_SCREEN, 3000);
  } else if (uiState == MESSAGE_DISPLAY) {
    messageDisplayUI.scrollUp();
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
          uiManager.showMessageAndReturn(" Alarm Set!", ALARM_OVERVIEW);
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
      showMessageAndReturn(" Alarm Stopped", IDLE_SCREEN, 3000);
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
    case MESSAGE_DISPLAY:
      // optional: handle message state
      break;
  }
}


UIState UIManager::getCurrentState() const {
  return currentState;
}

// show temp message on screen then return to Idle screen
// implementation
void UIManager::showMessageAndReturn(const String& message, UIState nextScreen, unsigned long durationMs) {
  temporaryMessage = message;
  temporaryScreenStart = millis();
  temporaryScreenDuration = durationMs;
  returnState = nextScreen;
  switchTo(MESSAGE_DISPLAY);
}
