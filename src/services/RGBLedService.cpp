// RGBLedService.cpp
#include "../include/services/RGBLedService.h"
#include "config.h"

#define CHANNEL_RED    0
#define CHANNEL_GREEN  1
#define CHANNEL_BLUE   2

void RGBLedService::begin() {
  ledcSetup(CHANNEL_RED, 5000, 8);
  ledcSetup(CHANNEL_GREEN, 5000, 8);
  ledcSetup(CHANNEL_BLUE, 5000, 8);

  ledcAttachPin(RED_PIN, CHANNEL_RED);
  ledcAttachPin(GREEN_PIN, CHANNEL_GREEN);
  ledcAttachPin(BLUE_PIN, CHANNEL_BLUE);

  turnOff();
}

void RGBLedService::setColor(uint8_t red, uint8_t green, uint8_t blue) {
  writeChannel(CHANNEL_RED, red);
  writeChannel(CHANNEL_GREEN, green);
  writeChannel(CHANNEL_BLUE, blue);
}

void RGBLedService::turnOff() {
  setColor(0, 0, 0);
}

void RGBLedService::writeChannel(uint8_t channel, uint8_t value) {
  ledcWrite(channel, value);
}
