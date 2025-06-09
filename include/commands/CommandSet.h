#pragma once
#include <Arduino.h>
#include <vector>

struct CommandSet {
  static const std::vector<String> greetings;
  static const std::vector<String> farewells;
  static const std::vector<String> grateful;
  static const std::vector<String> skeptical;
  static const std::vector<String> sleepy;  // ⬅️ Add this

};
