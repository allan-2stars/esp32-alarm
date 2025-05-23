#ifndef CONFIG_H
#define CONFIG_H

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SDA_PIN 16
#define SCL_PIN 17
#define RESET_BUTTON_PIN 32

#define DHTPIN 14
#define DHTTYPE DHT22

#define MODE_BUTTON_PIN     33
#define ADJUST_BUTTON_PIN   5
#define CONFIRM_BUTTON_PIN  4
#define BUZZER_PIN          15

#define UI_TIMEOUT_MS  30000  // half minute
#define HEADER_HEIGHT 10  // vertical space reserved for top icons
#define TEXT_COLOR SSD1306_WHITE

#define MAX_SCREEN_ALARMS 3
#define MAX_TOTAL_ALARMS 10
#define SCREEN_ALARM_VERSION 0xA1A1
#define WEB_ALARM_VERSION    0xB2B2

#define RED_PIN    18
#define GREEN_PIN  19
#define BLUE_PIN   23

#endif
