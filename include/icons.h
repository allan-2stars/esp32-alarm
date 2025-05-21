#ifndef ICONS_H
#define ICONS_H

#include <Adafruit_SSD1306.h>

void drawWifiIcon(Adafruit_SSD1306 &display, int x, int y);
void drawBtIcon(Adafruit_SSD1306 &display, int x, int y);
void drawBellIcon(Adafruit_SSD1306 &display, int x, int y);
void drawBellSlashIcon(Adafruit_SSD1306 &display, int x, int y);

extern const unsigned char bellBitmap32x32[];

#endif
