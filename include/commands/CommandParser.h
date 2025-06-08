#pragma once
#include <Arduino.h>

class CommandParser {
public:
  static String normalize(const String& input);
};
