#ifndef ICONS_H
#define ICONS_H

#include <Adafruit_SSD1306.h>

void drawWifiIcon(Adafruit_SSD1306 &display, int x, int y);
void drawBtIcon(Adafruit_SSD1306 &display, int x, int y);
void drawBellIcon(Adafruit_SSD1306 &display, int x, int y);
void drawBellSlashIcon(Adafruit_SSD1306 &display, int x, int y);

extern const unsigned char bellBitmap32x32[];

// Wi-Fi animation frames
extern const unsigned char wifi_1[];
extern const unsigned char wifi_2[];
extern const unsigned char wifi_3[];

// Moon icon
extern const unsigned char moon_icon[];

// Rotating sun frames (5 total)
extern const unsigned char sun_frames[5][32];
const int SUN_FRAME_COUNT = 5;

#endif