#pragma once
#include "FaceEmotion.h"

class HappyEmotion : public FaceEmotion {
public:
  void activate() override;
  const char* getName() const override { return "Happy"; }
};
