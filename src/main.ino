#include <Arduino.h>
#include "alarm.h"
#include "ui.h"
#include "buttons.h"
#include "utils.h"
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
  Wire.begin(16, 17);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.display();

  // ensures all alarms[i] start with the correct date and default time.
  for (int i = 0; i < 3; i++) {
    alarms[i].enabled = false;
    alarms[i].type = ONE_TIME;
    setAlarmToCurrentTime(alarms[i]);
  }


  // Connect Wi-Fi
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" WiFi connected");

  // Set timezone and NTP
  configTzTime("AEST-10AEDT,M10.1.0,M4.1.0/3", "pool.ntp.org");

  // ⏱️ Wait for NTP sync
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo) || timeinfo.tm_year + 1900 < 2024) {
    delay(200);
    Serial.println("Waiting for time sync...");
  }
  Serial.println("Time synced!");
  
  // after NTP wait
  for (int i = 0; i < 3; i++) {
    alarms[i].enabled = false;
    alarms[i].type = ONE_TIME;
    setAlarmToCurrentTime(alarms[i]);
  }

}


void loop() {
  handleButtons();          // input + screen update
  checkAndTriggerAlarms();  // time match + melody
  delay(50);
}
