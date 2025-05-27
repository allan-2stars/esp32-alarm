#include <Arduino.h>
#include "alarm.h"
#include "ui.h"
#include "buttons.h"
#include "melody_engine.h"
#include "utils.h"
#include "config.h"
#include "draw_bell.h"
#include "globals.h"
#include "alarm_storage.h"
#include "led.h"
#include "web_server.h"
#include "secrets.h"
#include "sensor.h"
#include "alarm_storage.h"
#include "animations.h"
//#include <Adafruit_GFX.h>


UIState uiState = IDLE_SCREEN;
Alarm alarms[MAX_SCREEN_ALARMS];
Alarm tempAlarm;
int selectedAlarmIndex = 0;
AlarmField selectedField = ALARM_TYPE;
int currentRepeatDayIndex = 0;
int previewMelodyIndex = 0;
bool lastSnoozed = false;
unsigned long messageDisplayStart = 0;

time_t snoozeUntil = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  //
  initButtons();
  initBuzzer();
  //
  initLED();
  initRGBLed();
  initAHT10();
  //
  initDisplay(display);

  // Connect to Wifi
  if(!connectWifi()){
    return;
  }

  initNTP();
  initAlarmStorage();
  //initFirebase();

}

void loop() {
  handleButtons();          // input + screen update
  checkAndTriggerAlarms();  // time match + melody
  updateMelodyPlayback();
  resetESP32(); // only trigger by press the reset button.
  updateAnimations();
  
  // Update display
  // if (isMelodyPlaying()) {
  //   setLedMode(LED_MELODY);
  // }
  // else{
  //   setLedMode(LED_OFF);
  // }
  switch (uiState) {
    case IDLE_SCREEN: drawIdleScreen(); break;
    case ALARM_OVERVIEW: drawAlarmOverview(); break;
    case ALARM_CONFIG: drawAlarmConfig(); break;
    case MELODY_PREVIEW: drawMelodyPreview(previewMelodyIndex); break;
    case ALARM_RINGING: drawBellRinging(display); break;
    // if lastSnoozed is true, then show snooze message, otherwise, show stop message.
    case ALARM_SNOOZE_MESSAGE: {
      drawSnoozeMessage(lastSnoozed); 
      uiState = IDLE_SCREEN;break;}
    case ERROR_SCREEN:
      drawErrorScreen();
      break;
  }

  //getDataFromFirebase();
  checkIdleAndSleep();  // ✅ Inserted here to handle idle timeout + sleep
  delay(50); // Small delay to prevent CPU overload
}