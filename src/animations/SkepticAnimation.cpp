#include "animations/SkepticAnimation.h"
#include "ui/RobotFaceUI.h"

SkepticAnimation::SkepticAnimation() {}

void SkepticAnimation::start(Face* f) {
  face = f;
  stage = SkepticStage::SKEPTIC;
  stageStart = millis();
  finished = false;

  if (!face) return;

  showEmotion("Skeptic");
  face->RandomBlink = false;
  face->RandomLook = false;
}

void SkepticAnimation::update(unsigned long now) {
  if (finished || !face) return;

  unsigned long elapsed = now - stageStart;

  switch (stage) {
    case SkepticStage::SKEPTIC:
      if (elapsed >= 2000) nextStage(now); 
      break;
    case SkepticStage::SUSPICIOUS:
      showEmotion("Suspicious");
      face->RandomBlink = true;
      face->RandomLook = true;
      if (elapsed >= 1000) nextStage(now);
    case SkepticStage::TRANSITION:
      face->DoBlink();
      nextStage(now);
      break;
    case SkepticStage::ANNOYED:
      showEmotion("Annoyed");
      if (elapsed >= 1500) nextStage(now);
      break;
    case SkepticStage::DONE:
      showEmotion("Normal");
      finished = true;
      break;
  }
}

void SkepticAnimation::nextStage(unsigned long now) {
  stage = static_cast<SkepticStage>(static_cast<int>(stage) + 1);
  stageStart = now;
}

bool SkepticAnimation::isFinished() const {
  return finished;
}


void SkepticAnimation::showEmotion(const char* name) {
  if (!face) return;
  if (strcmp(name, "Annoyed") == 0) {
    face->Expression.GoTo_Annoyed();
    face->RandomBlink = false;
  } else if (strcmp(name, "Suspicious") == 0) {
    face->Expression.GoTo_Suspicious();
    face->RandomBlink = false;
  } else if (strcmp(name, "Normal") == 0) {
    face->Expression.GoTo_Normal();
    face->RandomBlink = true;
  }else if (strcmp(name, "Skeptic") == 0) {
    face->Expression.GoTo_Skeptic();
  }
}
