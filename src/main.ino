#include <Arduino.h>
#include "globals.h"
#include "buttons.h"
#include "utils.h"
#include "sensor.h"
#include "web_server.h"

void setup() {
  Serial.begin(115200);

  Wire.begin(SDA_PIN, SCL_PIN);
  initButtons();
  initBuzzer();
  initAHT10();
  initDisplay(display);

  if (!connectWifi()) {
    display.clearDisplay();
    display.setTextColor(TEXT_COLOR);
    display.setCursor((SCREEN_WIDTH - 72) / 2, SCREEN_HEIGHT / 2 - 8);
    display.print("Internet connection failed!");
    display.display();
    return;
  }

  initNTP();

  alarmService.begin();
  melodyService.begin();
  ledService.begin();
  rgbLed.begin();
  rgbLed.setColor(255, 0, 0);
  alarmStorageService.begin();
  sunMoonUI.begin();
  shockSensor.begin();
}

void loop() {
  checkSerialCommand();     // 👈 check for serial triggers first

  alarmService.update();
  melodyService.update();
  handleButtons();
  resetESP32();
  alarmService.checkAlarms();
  alarmService.handleSnooze();
  uiManager.update();
  //shockSensor.update();
  
  // if (shockSensor.isShocked()) {
  //   Serial.println("detected");
  //   ledcWrite(PWM_CHANNEL, 255);          // Full brightness
  // } else {
  //   ledcWrite(PWM_CHANNEL, 0);     
  //   Serial.println("Not detected");       // LED off
  // }

  if (uiManager.getCurrentState() != ALARM_RINGING &&
      millis() - lastInteraction > INACTIVITY_TIMEOUT) {
    sleepManager.update();
  }

  delay(50);
}


