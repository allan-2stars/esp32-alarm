#pragma once
#include "FaceEmotion.h"

class NormalEmotion : public FaceEmotion {
public:
  void activate() override;
  const char* getName() const override { return "Normal"; }
};
