#ifndef CONFIG_H
#define CONFIG_H

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_RESET -1
#define SDA_PIN 16
#define SCL_PIN 17

#define DHTPIN 14
#define DHTTYPE DHT22

#define MODE_BUTTON_PIN     33
#define ADJUST_BUTTON_PIN   5
#define CONFIRM_BUTTON_PIN  4
#define BUZZER_PIN          15

#define UI_TIMEOUT_MS       60000  // 1 minute
#define HEADER_HEIGHT 10  // vertical space reserved for top icons
#define TEXT_COLOR SSD1306_WHITE



#endif
