#pragma once
#include "FaceEmotion.h"

class FocusedEmotion : public FaceEmotion {
public:
  void activate() override;
  const char* getName() const override { return "Focused"; }
};
