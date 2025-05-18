#include <WiFi.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <time.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SDA_PIN       16
#define SCL_PIN       17

#define DHTPIN        14
#define DHTTYPE       DHT22

#define MODE_BUTTON_PIN     33
#define ADJUST_BUTTON_PIN   5
#define SELECT_BUTTON_PIN   4

#define BUZZER_PIN          15
#define UI_TIMEOUT_MS       60000

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
DHT dht(DHTPIN, DHTTYPE);

enum UIState {
  IDLE_SCREEN,
  ALARM_OVERVIEW,
  ALARM_CONFIG
};

enum AlarmField {
  ALARM_TYPE,
  ALARM_TIME_HOUR,
  ALARM_TIME_MIN,
  ALARM_DATE_YEAR,
  ALARM_DATE_MONTH,
  ALARM_DATE_DAY,
  ALARM_REPEAT_DAYS,
  ALARM_ENABLED,
  ALARM_MELODY,
  ALARM_SAVE_EXIT
};

enum AlarmType { ONE_TIME, SPECIFIC_DATE, REPEATED };

struct Alarm {
  bool enabled = false;
  AlarmType type = ONE_TIME;
  int hour = 7;
  int minute = 0;
  int year = 2025;
  int month = 5;
  int day = 18;
  bool repeatDays[7] = {false};
  int melody = 0;
  String name = "";
};

Alarm alarms[3];
UIState uiState = IDLE_SCREEN;
unsigned long lastInteractionTime = 0;
unsigned long lastModePress = 0, lastAdjustPress = 0, lastSelectPress = 0;
const unsigned long debounceDelay = 200;

int selectedAlarmIndex = 0;
AlarmField selectedField = ALARM_TYPE;
bool editingField = false;
int currentRepeatDayIndex = 0;

const char* melodyNames[] = {"Mario", "Birthday", "Xmas"};
const char* weekDays[] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};

bool isFieldVisible(AlarmType type, AlarmField field) {
  if (field == ALARM_DATE_YEAR || field == ALARM_DATE_MONTH || field == ALARM_DATE_DAY)
    return type == SPECIFIC_DATE;
  if (field == ALARM_REPEAT_DAYS)
    return type == REPEATED;
  return true;
}

void connectToWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin("Wokwi-GUEST", "", 6);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println(" Connected!");
  configTzTime("AEST-10AEDT,M10.1.0,M4.1.0/3", "pool.ntp.org");
}

String getFormattedTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "Time Err";
  char buf[6];
  strftime(buf, sizeof(buf), "%H:%M", &timeinfo);
  return String(buf);
}

String getFormattedDate() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "Date Err";
  char buf[20];
  strftime(buf, sizeof(buf), "%a %Y-%m-%d", &timeinfo);
  return String(buf);
}

void drawIdleScreen() {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print(getFormattedTime());

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
  display.print(getFormattedDate());
  display.display();
}

void drawAlarmOverview() {
  display.clearDisplay();
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

void drawAlarmConfig() {
  Alarm &a = alarms[selectedAlarmIndex];
  display.clearDisplay();
  display.setTextSize(1);
  int y = 0;

  display.setCursor(0, y); y += 10;
  display.printf("Config A%d", selectedAlarmIndex + 1);

  if (isFieldVisible(a.type, ALARM_TYPE)) {
    display.setCursor(0, y); y += 10;
    display.printf("%sType: %s", 
      selectedField == ALARM_TYPE ? ">" : " ",
      a.type == ONE_TIME ? "Once" : a.type == SPECIFIC_DATE ? "Date" : "Repeat");
  }

  display.setCursor(0, y); y += 10;
  display.printf("%sTime: %02d:%02d", 
    (selectedField == ALARM_TIME_HOUR || selectedField == ALARM_TIME_MIN) ? ">" : " ",
    a.hour, a.minute);

  if (isFieldVisible(a.type, ALARM_DATE_YEAR)) {
    display.setCursor(0, y); y += 10;
    display.printf("%sDate: %04d-%02d-%02d",
      (selectedField == ALARM_DATE_YEAR || selectedField == ALARM_DATE_MONTH || selectedField == ALARM_DATE_DAY) ? (editingField ? "*" : ">") : " ",
      a.year, a.month, a.day);
  }

  if (isFieldVisible(a.type, ALARM_REPEAT_DAYS)) {
    display.setCursor(0, y); y += 10;
    display.print((selectedField == ALARM_REPEAT_DAYS) ? (editingField ? "*Days: " : ">Days: ") : " Days: ");
    for (int i = 0; i < 7; i++) {
      if (i == currentRepeatDayIndex && selectedField == ALARM_REPEAT_DAYS) display.print("[");
      display.print(weekDays[i][0]);
      if (a.repeatDays[i]) display.print("*");
      else display.print(" ");
      if (i == currentRepeatDayIndex && selectedField == ALARM_REPEAT_DAYS) display.print("]");
      display.print(" ");
    }
  }

  display.setCursor(0, y); y += 10;
  display.printf("%sEnabled: %s", selectedField == ALARM_ENABLED ? ">" : " ", a.enabled ? "Yes" : "No");

  display.setCursor(0, y);
  display.printf("%sMelody: %s", selectedField == ALARM_MELODY ? ">" : " ", melodyNames[a.melody]);


  display.display();
}

void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);
  pinMode(MODE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(ADJUST_BUTTON_PIN, INPUT_PULLUP);
  pinMode(SELECT_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  dht.begin();
  connectToWiFi();
}

void loop() {
  unsigned long now = millis();

  if (digitalRead(MODE_BUTTON_PIN) == LOW && now - lastModePress > debounceDelay) {
    lastModePress = now;
    if (uiState == ALARM_OVERVIEW) {
      uiState = ALARM_CONFIG;
      selectedField = ALARM_TYPE;
      editingField = false;
    } else if (uiState == ALARM_CONFIG) {
      do {
        selectedField = (AlarmField)((selectedField + 1) % 10);
      } while (!isFieldVisible(alarms[selectedAlarmIndex].type, selectedField));
      editingField = false;
    } else {
      uiState = (UIState)((uiState + 1) % 3);
    }
    lastInteractionTime = now;
  }

  if (digitalRead(ADJUST_BUTTON_PIN) == LOW && now - lastAdjustPress > debounceDelay) {
    lastAdjustPress = now;
    Alarm &a = alarms[selectedAlarmIndex];
    switch (selectedField) {
      case ALARM_TYPE: a.type = (AlarmType)((a.type + 1) % 3); break;
      case ALARM_TIME_HOUR: a.hour = (a.hour + 1) % 24; break;
      case ALARM_TIME_MIN: a.minute = (a.minute + 1) % 60; break;
      case ALARM_DATE_YEAR: a.year = (a.year < 2035) ? a.year + 1 : 2025; break;
      case ALARM_DATE_MONTH: a.month = (a.month % 12) + 1; break;
      case ALARM_DATE_DAY: a.day = (a.day % 31) + 1; break;
      case ALARM_REPEAT_DAYS: currentRepeatDayIndex = (currentRepeatDayIndex + 1) % 7; break;
      case ALARM_ENABLED: a.enabled = !a.enabled; break;
      case ALARM_MELODY: a.melody = (a.melody + 1) % 3; break;
      default: break;
    }
    lastInteractionTime = now;
  }

  if (digitalRead(SELECT_BUTTON_PIN) == LOW && now - lastSelectPress > debounceDelay) {
    lastSelectPress = now;
    lastInteractionTime = now;
    if (uiState == ALARM_OVERVIEW) {
      selectedAlarmIndex = (selectedAlarmIndex + 1) % 3;
    } else if (uiState == ALARM_CONFIG) {
      if (selectedField == ALARM_REPEAT_DAYS) {
        alarms[selectedAlarmIndex].repeatDays[currentRepeatDayIndex] = 
          !alarms[selectedAlarmIndex].repeatDays[currentRepeatDayIndex];
      } else {
        editingField = !editingField;
      }
    }
  }

  if (now - lastInteractionTime > UI_TIMEOUT_MS && uiState != IDLE_SCREEN) {
    uiState = IDLE_SCREEN;
  }

  switch (uiState) {
    case IDLE_SCREEN: drawIdleScreen(); break;
    case ALARM_OVERVIEW: drawAlarmOverview(); break;
    case ALARM_CONFIG: drawAlarmConfig(); break;
  }

  delay(50);
}
