#include "commands/CommandRouter.h"
#include "commands/CommandSet.h"
#include "globals.h"

void CommandRouter::handleCommand(const String& cmd) {
  for (const auto& phrase : CommandSet::greetings) {
    if (cmd == phrase) {
      Serial.println("✅ Greeting detected");
      uiManager.switchTo(ROBOT_FACE_DISPLAY);
      return;
    }
  }

  for (const auto& phrase : CommandSet::farewells) {
    if (cmd == phrase) {
      Serial.println("👋 Goodbye detected");
      uiManager.switchTo(IDLE_SCREEN);
      return;
    }
  }

  for (const auto& phrase : CommandSet::grateful) {
    if (cmd == phrase) {
      Serial.println("🙏 Grateful command detected");

      // Trigger animation (you'll implement this in RobotFaceUI)
      robotFaceUI.playGratefulAnimation();
      return;
    }
  }


  Serial.print("❓ Unknown command: ");
  Serial.println(cmd);
}
