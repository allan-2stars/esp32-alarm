#pragma once
#include "FaceEmotion.h"

class FrustratedEmotion : public FaceEmotion {
public:
  void activate() override;
  const char* getName() const override { return "Frustrated"; }
};
