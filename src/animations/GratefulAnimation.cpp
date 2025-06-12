#include "animations/GratefulAnimation.h"
#include "ui/RobotFaceUI.h"

GratefulAnimation::GratefulAnimation() {}

void GratefulAnimation::start(Face* f) {
  face = f;
  stage = GratefulStage::AWE;
  stageStart = millis();
  finished = false;
  showEmotion("Awe");
}

void GratefulAnimation::update(unsigned long now) {
  if (finished || !face) return;
  unsigned long elapsed = now - stageStart;
  switch (stage) {
    case GratefulStage::AWE:
      if (elapsed >= 1000) nextStage(now);
      break;
    case GratefulStage::TRANSITION:
      face->DoBlink();
      nextStage(now);
      break;
    case GratefulStage::HAPPY1:
      showEmotion("Happy");
      if (elapsed >= 2000) nextStage(now);
      break;
    case GratefulStage::BLINK1:
      face->DoBlink();
      nextStage(now);
      break;
    case GratefulStage::HAPPY2:
      showEmotion("Happy");
      if (elapsed >= 2000) nextStage(now);
      break;
    case GratefulStage::BLINK2:
      face->DoBlink();
      nextStage(now);
      break;
    case GratefulStage::DONE:
      showEmotion("Normal");
      finished = true;
      break;
  }
}

void GratefulAnimation::nextStage(unsigned long now) {
  stage = static_cast<GratefulStage>(static_cast<int>(stage) + 1);
  stageStart = now;
}

bool GratefulAnimation::isFinished() const {
  return finished;
}

void GratefulAnimation::showEmotion(const char* name) {
  if (!face) return;
  if (strcmp(name, "Awe") == 0) {
    face->Expression.GoTo_Awe();
    face->RandomBlink = false;
  } else if (strcmp(name, "Happy") == 0) {
    face->Expression.GoTo_Happy();
    face->RandomBlink = false;
  } else if (strcmp(name, "Normal") == 0) {
    face->Expression.GoTo_Normal();
    face->RandomBlink = true;
  }
}
