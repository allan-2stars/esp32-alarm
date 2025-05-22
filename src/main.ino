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
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <nvs_flash.h>
#include "Preferences.h"
Preferences prefs;
UIState uiState = IDLE_SCREEN;
Alarm alarms[MAX_SCREEN_ALARMS];
Alarm tempAlarm;
int selectedAlarmIndex = 0;
AlarmField selectedField = ALARM_TYPE;
int currentRepeatDayIndex = 0;
int previewMelodyIndex = 0;
bool lastSnoozed = false;
unsigned long messageDisplayStart = 0;
String errorMessage = "";
time_t snoozeUntil = 0;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
//####

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RESET_BUTTON_PIN, INPUT_PULLUP);
  digitalWrite(BUZZER_PIN, LOW);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(TEXT_COLOR);

  // Connect to Wi-Fi
  WiFi.begin("Wokwi-GUEST", "", 6);
  unsigned long wifiStart = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStart < 6000) {
    delay(100);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    errorMessage = "Connection failed.\n Check your WiFi.";
    uiState = ERROR_SCREEN;
    return;
  }
  Serial.println("WiFi connected");

  // Configure timezone and try to get time
  configTzTime("AEST-10AEDT,M10.1.0,M4.1.0/3", "pool.ntp.org");

  struct tm timeinfo;
  bool timeSynced = false;
  unsigned long ntpStart = millis();
  while (millis() - ntpStart < 10000) {
    if (getLocalTime(&timeinfo) && timeinfo.tm_year + 1900 >= 2024) {
      timeSynced = true;
      break;
    }
    delay(200);
  }

  if (!timeSynced) {
    Serial.println("NTP sync failed.");
    display.clearDisplay();
    display.setCursor(10, 20);
    display.println("Time sync failed.");
    display.setCursor(10, 30);
    display.println("Check WiFi or reset.");
    display.display();
    while (true); // Halt execution until reset
  }

  Serial.println("Time synced!");

  // Initialize storage
  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    Serial.println("NVS corrupted. Erasing...");
    nvs_flash_erase();
    nvs_flash_init();
  }

  // Load alarms or fallback
  for (int i = 0; i < MAX_SCREEN_ALARMS; i++) {
    Alarm a;
    loadAlarm(a, i);
    if (a.version != SCREEN_ALARM_VERSION && a.version != WEB_ALARM_VERSION) {
      a.hour = 7;
      a.minute = 0;
      a.year = timeinfo.tm_year + 1900;
      a.month = timeinfo.tm_mon + 1;
      a.day = timeinfo.tm_mday;
      a.enabled = false;
      a.type = ONE_TIME;
      a.melody = 0;
      setAlarmToCurrentTime(a);
    }
    alarms[i] = a;
  }
}


void loop() {
  handleButtons();          // input + screen update
  checkAndTriggerAlarms();  // time match + melody
  updateMelodyPlayback();

  // Update display
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

  // This is the Test button, will be removed for real project.
  if (digitalRead(RESET_BUTTON_PIN) == LOW) {
    delay(50);  // simple debounce
    ESP.restart();
  }

  delay(50);
}
