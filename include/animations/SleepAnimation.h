#pragma once
#include "animations/FaceAnimation.h"

enum class SleepStage {
  BLINK1,
  BLINK2,
  SLEEPY_EYES,
  FINAL_BLINK,
  FAREWELL,
  DONE
};

class SleepAnimation : public FaceAnimation {
public:
  SleepAnimation();
  void start(Face* face) override;
  void update(unsigned long now) override;
  bool isFinished() const override;

private:
  Face* face = nullptr;
  SleepStage stage = SleepStage::DONE;
  unsigned long stageStart = 0;
  bool finished = true;

  void nextStage(unsigned long now);
  void showEmotion(const char* name);
};
