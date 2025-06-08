#pragma once
#include "FaceEmotion.h"

class FuriousEmotion : public FaceEmotion {
public:
  void activate() override;
  const char* getName() const override { return "Furious"; }
};
