
#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SDA_PIN       16
#define SCL_PIN       17

#define DHTPIN        14
#define DHTTYPE       DHT22

// Button pins
#define MODE_BUTTON_PIN     33
#define ADJUST_BUTTON_PIN   5
#define CONFIRM_BUTTON_PIN  4

// Buzzer pin
#define BUZZER_PIN          15

// UI timeout
#define UI_TIMEOUT_MS       60000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

enum UIState {
  IDLE_SCREEN,
  ALARM_OVERVIEW,
  ALARM_CONFIG
};

UIState uiState = IDLE_SCREEN;
unsigned long lastInteractionTime = 0;

// Placeholder struct for future alarm settings
struct Alarm {
  bool enabled = false;
  String type = "One-time";  // "One-time", "Repeat", "Date"
  int hour = 7;
  int minute = 0;
  String name = "";
};

Alarm alarms[3];

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");
}

void drawIdleScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("12:34");

  display.setTextSize(1);
  display.setCursor(0, 20);
  float temp = dht.readTemperature();
  float hum = dht.readHumidity();
  display.printf("T:%.1fC H:%.1f%%", temp, hum);

  display.setCursor(0, 32);
  bool anyAlarmEnabled = false;
  for (int i = 0; i < 3; i++) if (alarms[i].enabled) anyAlarmEnabled = true;
  display.print(anyAlarmEnabled ? "Alarm ON" : "Alarm OFF");

  display.setCursor(0, 52);
  display.print("Mon 2025-05-18");

  display.display();
}

void drawAlarmOverview() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Alarms");

  for (int i = 0; i < 3; i++) {
    display.setCursor(0, 12 + i * 16);
    display.printf("A%d: %02d:%02d %s", i + 1,
                   alarms[i].hour, alarms[i].minute,
                   alarms[i].enabled ? "ON" : "OFF");
  }
  display.display();
}


unsigned long lastModePress = 0;
unsigned long lastAdjustPress = 0;
unsigned long lastConfirmPress = 0;
const unsigned long debounceDelay = 200;

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(CONFIRM_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }
  display.clearDisplay();
  dht.begin();
  connectToWiFi();
}


void loop() {
  // Debounced Mode button
  if (digitalRead(MODE_BUTTON_PIN) == LOW && millis() - lastModePress > debounceDelay) {
    lastModePress = millis();
    uiState = (UIState)((uiState + 1) % 3);
    lastInteractionTime = millis();
  }

  // Debounced Adjust button (to be used in alarm config)
  if (digitalRead(ADJUST_BUTTON_PIN) == LOW && millis() - lastAdjustPress > debounceDelay) {
    lastAdjustPress = millis();
    // Placeholder for adjust button action
    Serial.println("Adjust button pressed");
    lastInteractionTime = millis();
  }

  // Debounced Confirm button (to be used in alarm config)
  if (digitalRead(CONFIRM_BUTTON_PIN) == LOW && millis() - lastConfirmPress > debounceDelay) {
    lastConfirmPress = millis();
    // Placeholder for confirm button action
    Serial.println("Confirm button pressed");
    lastInteractionTime = millis();
  }

  // Timeout logic
  if (millis() - lastInteractionTime > UI_TIMEOUT_MS && uiState != IDLE_SCREEN) {
    uiState = IDLE_SCREEN;
  }

  // UI Rendering
  switch (uiState) {
    case IDLE_SCREEN:
      drawIdleScreen();
      break;
    case ALARM_OVERVIEW:
      drawAlarmOverview();
      break;
    case ALARM_CONFIG:
      display.clearDisplay();
      display.setCursor(0, 0);
      display.print("Alarm Config Screen");
      display.display();
      break;
  }

  delay(50); // Responsive but not too fast
}


  
