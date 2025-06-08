#pragma once
#include "animations/FaceAnimation.h"

enum class GratefulStage {
  AWE,
  TRANSITION,
  HAPPY1,
  BLINK1,
  HAPPY2,
  BLINK2,
  DONE
};

class GratefulAnimation : public FaceAnimation {
public:
  GratefulAnimation();
  void start(Face* face) override;
  void update(unsigned long now) override;
  bool isFinished() const override;

private:
  Face* face = nullptr;
  GratefulStage stage = GratefulStage::DONE;
  unsigned long stageStart = 0;
  bool finished = true;

  void nextStage(unsigned long now);
  void showEmotion(const char* name);
};
