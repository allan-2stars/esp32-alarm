#pragma once

#include <Adafruit_SSD1306.h>

void drawWifiIcon(Adafruit_SSD1306 &display, int x, int y);
void drawBtIcon(Adafruit_SSD1306 &display, int x, int y);
void drawBellIcon(Adafruit_SSD1306 &display, int x, int y);
void drawBellSlashIcon(Adafruit_SSD1306 &display, int x, int y);

extern const unsigned char wake_up_91x35[];
extern const unsigned char bell_ringing_39x35[];
extern const unsigned char doraemon[];
// Wi-Fi animation frames
extern const unsigned char wifi_1[];
extern const unsigned char wifi_2[];
extern const unsigned char wifi_3[];

// Moon icosn
extern const unsigned char moon_icon_25x25[];

// Rotating sun frames (5 total)
extern const unsigned char sun_frames[4][100];
const int SUN_FRAME_COUNT = 5;

// Bell
extern const unsigned char bellIcon[];
extern const unsigned char bellSlashIcon[];
