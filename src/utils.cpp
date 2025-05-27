#include <time.h>
#include "utils.h"
#include "config.h"
#include "led.h"
#include "ui.h"
#include <WiFi.h>
#include <WiFiManager.h>

String errorMessage = "";
unsigned long lastInteraction = 0;

int getCurrentYear() {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    return timeinfo.tm_year + 1900;
  }
  return 2025;
}

int getMaxDay(int year, int month) {
  struct tm t = {0};
  t.tm_year = year - 1900;
  t.tm_mon = month;
  t.tm_mday = 0;
  mktime(&t);
  return t.tm_mday;
}

bool isTimeAvailable() {
  struct tm t;
  return getLocalTime(&t) && (t.tm_year + 1900 >= 2024);
}

void setAlarmToCurrentTime(Alarm &a) {
  struct tm timeinfo;
  if (getLocalTime(&timeinfo)) {
    a.hour = 7;
    a.minute = 0;
    a.year = timeinfo.tm_year + 1900;
    a.month = timeinfo.tm_mon + 1;
    a.day = timeinfo.tm_mday;
  } else {
    // Fallback default time if NTP fails
    a.hour = 9;
    a.minute = 9;
    a.year = 2020;
    a.month = 9;
    a.day = 9;
  }

  // Unified default alarm settings
  a.enabled = false;
  a.type = ONE_TIME;
  a.melody = 0;
  a.version = SCREEN_ALARM_VERSION;  // Assign current version
}


String getFormattedTime() { 
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "Time Err";
  char buf[9];  // Enough space for "HH:MM:SS"
  strftime(buf, sizeof(buf), "%H:%M:%S", &timeinfo); 
  return String(buf);
}


String getFormattedDate() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return "Date Err";
  char buf[20]; strftime(buf, sizeof(buf), "%a %Y-%m-%d", &timeinfo);
  return String(buf);
}

bool connectWifi() {
  Serial.println("🔌 Starting Wi-Fi connection...");

  WiFiManager wm;

  // Optional: reset settings if needed for debugging
  // wm.resetSettings();

  // Auto-connect or open config portal
  // For Wokwi Wifi 
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  if (!wm.autoConnect("ESP32_Config", "config123")) {
    Serial.println("⚠️ AutoConnect failed. Starting config portal...");
    wm.startConfigPortal("ESP32_Config", "config123");
  }

  // Wait for Wi-Fi to connect (with timeout)
  unsigned long startAttemptTime = millis();
  const unsigned long timeout = 6000;

  while (WiFi.status() != WL_CONNECTED && millis() - startAttemptTime < timeout) {
    delay(100);
    Serial.print(".");
    setLedMode(LED_WIFI);  // blink LED while waiting
  }

  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\n❌ Wi-Fi connection failed.");
    errorMessage = "Connection failed.\nCheck your WiFi.";
    uiState = ERROR_SCREEN;
    return false;
  }

  Serial.println("\n✅ Wi-Fi connected!");
  Serial.print("📶 IP Address: ");
  Serial.println(WiFi.localIP());

  return true;
}

// if the list cannot be displayed in the screen, scroll down.
int adjustVisibleStart(int selectedIndex, int visibleStart, int maxVisible, int totalItems) {
    if (selectedIndex < visibleStart) {
        return selectedIndex;
    } else if (selectedIndex >= visibleStart + maxVisible) {
        return selectedIndex - maxVisible + 1;
    }
    return visibleStart;
}

void recordInteraction() {
  lastInteraction = millis();
}
