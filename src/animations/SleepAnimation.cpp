#include "animations/SleepAnimation.h"
#include "ui/RobotFaceUI.h"
#include "ui/UIManager.h"
extern UIManager uiManager;  // Ensure this is declared

SleepAnimation::SleepAnimation() {}

void SleepAnimation::start(Face* f) {
  face = f;
  stage = SleepStage::BLINK1;
  stageStart = millis();
  finished = false;
  face->RandomBlink = false;
}

void SleepAnimation::update(unsigned long now) {
  if (finished || !face) return;
  unsigned long elapsed = now - stageStart;

  switch (stage) {
    case SleepStage::BLINK1:
      face->DoBlink();
      nextStage(now);
      break;

    case SleepStage::BLINK2:
      face->DoBlink();
      nextStage(now);
      break;

    case SleepStage::SLEEPY_EYES:
      showEmotion("Sleepy");
      if (elapsed >= 1500) nextStage(now);
      break;

    case SleepStage::FINAL_BLINK:
      face->DoBlink();
      nextStage(now);
      break;

    case SleepStage::FAREWELL:
      if (elapsed >= 1000) {
        Serial.println("👋 Sleep animation complete. Exiting face mode.");
        uiManager.switchTo(IDLE_SCREEN);  // 👈 This replaces GoTo_Farewells()
        nextStage(now);
      }
      break;

    case SleepStage::DONE:
      finished = true;
      break;
  }
}


void SleepAnimation::nextStage(unsigned long now) {
  stage = static_cast<SleepStage>(static_cast<int>(stage) + 1);
  stageStart = now;
}

bool SleepAnimation::isFinished() const {
  return finished;
}

void SleepAnimation::showEmotion(const char* name) {
  if (!face) return;

  if (strcmp(name, "Sleepy") == 0) {
    face->Expression.GoTo_Sleepy();  // Make sure GoTo_Sleepy is implemented
    }
}
