#pragma once
#include "FaceEmotion.h"

class AngryEmotion : public FaceEmotion {
public:
  void activate() override;
  const char* getName() const override { return "Angry"; }
};
