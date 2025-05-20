#include <Adafruit_SSD1306.h>
#include "icons.h"
#include "config.h"

// Wi-Fi Icon 8x8
const unsigned char wifiIcon[] PROGMEM = {
  0b00011000,
  0b00111100,
  0b01111110,
  0b11011011,
  0b00011000,
  0b00011000,
  0b00000000,
  0b00011000
};

void drawWifiIcon(Adafruit_SSD1306 &display, int x, int y) {
  display.drawBitmap(x, y, wifiIcon, 8, 8, TEXT_COLOR);
}

// Bluetooth Icon 8x8
const unsigned char btIcon[] PROGMEM = {
  0b10000001,
  0b01000010,
  0b00100100,
  0b00011000,
  0b00011000,
  0b00100100,
  0b01000010,
  0b10000001
};

void drawBtIcon(Adafruit_SSD1306 &display, int x, int y) {
  display.drawBitmap(x, y, btIcon, 8, 8, TEXT_COLOR);
}

// Bell Icon 8x8
const unsigned char bellIcon[] PROGMEM = {
  0b00111000,
  0b01000100,
  0b10000010,
  0b10000010,
  0b10000010,
  0b01000100,
  0b00111000,
  0b00010000
};


void drawBellIcon(Adafruit_SSD1306 &display, int x, int y) {
  display.drawBitmap(x, y, bellIcon, 8, 8, TEXT_COLOR);
}

// Bell Slash Icon 8x8
const unsigned char bellSlashIcon[] PROGMEM = {
  0b00111000,
  0b01000100,
  0b10100010,
  0b10010010,
  0b10001010,
  0b01010100,
  0b00111000,
  0b00010000
};

void drawBellSlashIcon(Adafruit_SSD1306 &display, int x, int y) {
  display.drawBitmap(x, y, bellSlashIcon, 8, 8, TEXT_COLOR);
}
