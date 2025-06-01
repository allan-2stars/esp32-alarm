// RGBLedService.h
#pragma once

#include <Arduino.h>

class RGBLedService {
public:
  void begin();
  void setColor(uint8_t red, uint8_t green, uint8_t blue);
  void turnOff();

private:
  void writeChannel(uint8_t channel, uint8_t value);
};
