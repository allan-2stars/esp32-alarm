#include "commands/CommandSet.h"

const std::vector<String> CommandSet::greetings = {
  "hello pebble", "hi pebble", "wake up pebble", "show face", "face on"
};

const std::vector<String> CommandSet::farewells = {
  "goodbye pebble", "bye pebble", "sleep now", "hide face", "face off"
};

const std::vector<String> CommandSet::grateful = {
  "thank you",
  "thanks",
  "you helped a lot",
  "you are amazing",
  "i appreciate it",
  "you are awesome"
};

const std::vector<String> CommandSet::skeptical = {
 "really?", "seriously?", "you sure?", "doubt it", "hmph"
};

