#pragma once
#include "animations/FaceAnimation.h"
#include "Face.h"

enum class SkepticStage {
  SKEPTIC,
  SUSPICIOUS,
  TRANSITION,
  ANNOYED,
  DONE
};

class SkepticAnimation : public FaceAnimation {
public:
  SkepticAnimation();
  void start(Face* face) override;
  void update(unsigned long now) override;
  bool isFinished() const override;

private:
  Face* face = nullptr;
  SkepticStage stage = SkepticStage::DONE;
  unsigned long stageStart = 0;
  bool finished = true;

  void nextStage(unsigned long now);
  void showEmotion(const char* name);
};
