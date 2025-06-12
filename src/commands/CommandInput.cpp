#include <Arduino.h>
#include "commands/CommandInput.h"
#include "commands/CommandParser.h"
#include "commands/CommandRouter.h"

void checkSerialCommand() {
  if (Serial.available()) {
    String raw = Serial.readStringUntil('\n');
    String normalized = CommandParser::normalize(raw);
    CommandRouter::handleCommand(normalized);
  }
}
