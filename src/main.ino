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


  if (uiManager.getCurrentState() != ALARM_RINGING &&
      millis() - lastInteraction > INACTIVITY_TIMEOUT) {
    sleepManager.update();
  }

  delay(50);
}


void checkSerialCommand() {
  if (Serial.available()) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    // Convert to lowercase for case-insensitive comparison
    command.toLowerCase();

    // Greeting triggers
    if (command == "hello pebble" ||
        command == "hi pebble" ||
        command == "wake up pebble" ||
        command == "show face" ||
        command == "face on") {
      Serial.println("Activating robot face.");
      uiManager.switchTo(ROBOT_FACE_DISPLAY);
    }

    // Goodbye triggers
    else if (command == "goodbye pebble" ||
             command == "bye pebble" ||
             command == "sleep now" ||
             command == "hide face" ||
             command == "face off") {
      Serial.println("Exiting robot face.");
      uiManager.switchTo(IDLE_SCREEN);
    }

    else {
      Serial.print("Unknown command: ");
      Serial.println(command);
    }
  }
}

