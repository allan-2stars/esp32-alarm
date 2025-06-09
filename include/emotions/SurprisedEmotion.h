#pragma once
#include "FaceEmotion.h"

class SurprisedEmotion : public FaceEmotion {
public:
  void activate() override;
  const char* getName() const override { return "Surprised"; }
};
