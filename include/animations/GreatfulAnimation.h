// include/animations/GratefulAnimation.h
#pragma once
#include "animations/FaceAnimation.h"

class GratefulAnimation : public FaceAnimation {
public:
  void start(Face* face) override;
  void update(unsigned long now) override;
  bool isFinished() const override;

private:
  Face* face = nullptr;
  int step = 0;
  unsigned long stepStart = 0;
  bool finished = false;
};
