#pragma once
#include <Arduino.h>

class CommandRouter {
public:
  static void handleCommand(const String& normalizedCommand);
};
