#include "commands/CommandParser.h"

String CommandParser::normalize(const String& input) {
  String cmd = input;
  cmd.trim();
  cmd.toLowerCase();
  return cmd;
}
