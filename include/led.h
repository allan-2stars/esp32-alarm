#ifndef LED_H
#define LED_H

#include <Arduino.h>

enum LedMode {
  LED_OFF,
  LED_WIFI,
  LED_MELODY,
  LED_CUSTOM
};

void setLedMode(LedMode mode);

void initRGBLed();
void setLedColor(uint8_t r, uint8_t g, uint8_t b);
void blinkLed(uint8_t r, uint8_t g, uint8_t b, unsigned long intervalMs);
void updateLed();
void initLED();

void updateMoonLedFade(bool enabled);  // call this only at night

#endif
