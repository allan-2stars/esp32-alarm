#pragma once

#include "FaceEmotion.h"

class ScaredEmotion : public FaceEmotion {
public:
  void activate() override;
  const char* getName() const override { return "Scared"; }
};
