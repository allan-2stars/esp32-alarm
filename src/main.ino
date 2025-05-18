#include <Arduino.h>
#include "alarm.h"
#include "ui.h"
#include "buttons.h"
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


Adafruit_SSD1306 display(128, 64, &Wire, -1);

Alarm alarms[3];
Alarm tempAlarm;

int selectedAlarmIndex = 0;
AlarmField selectedField = ALARM_TYPE;
int currentRepeatDayIndex = 0;

UIState uiState = IDLE_SCREEN;

void setup() {
  Serial.begin(115200);
  Wire.begin(16, 17);  // SDA = 16, SCL = 17
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  // Add Wi-Fi and time sync here
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) delay(100);
  configTzTime("AEST-10AEDT,M10.1.0,M4.1.0/3", "pool.ntp.org");
}

void loop() {
  handleButtons();          // input + screen update
  checkAndTriggerAlarms();  // time match + melody
  delay(50);
}
