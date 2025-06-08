#include "commands/CommandRouter.h"
#include "commands/CommandSet.h"
#include "globals.h"
void CommandRouter::handleCommand(const String& cmd) {
  UIState currentState = uiManager.getCurrentState();

  // Greetings: enter face mode
  for (const auto& phrase : CommandSet::greetings) {
    if (cmd == phrase) {
      Serial.println("✅ Greeting detected");

      // Only switch if not already in face screen
      if (currentState != ROBOT_FACE_DISPLAY) {
        uiManager.switchTo(ROBOT_FACE_DISPLAY);
      }

      return;
    }
  }

  // Farewells: leave face mode
  for (const auto& phrase : CommandSet::farewells) {
    if (cmd == phrase) {
      Serial.println("👋 Goodbye detected");

      // Only exit face mode if you're in it
      if (currentState == ROBOT_FACE_DISPLAY) {
        uiManager.switchTo(IDLE_SCREEN);
      }

      return;
    }
  }

  // Grateful: play special animation (only if already in face screen)
  for (const auto& phrase : CommandSet::grateful) {
    if (cmd == phrase) {
      Serial.println("🙏 Grateful command detected");

      if (currentState == ROBOT_FACE_DISPLAY) {
        robotFaceUI.playGratefulAnimation();
      } else {
        Serial.println("⚠️ Ignored: Not in Robot Face screen");
      }

      return;
    }
  }

  // Other future command types go here...

  Serial.print("❓ Unknown command: ");
  Serial.println(cmd);
}
