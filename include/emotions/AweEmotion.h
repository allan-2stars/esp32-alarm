#pragma once
#include "FaceEmotion.h"

class AweEmotion : public FaceEmotion {
public:
  void activate() override;
  const char* getName() const override { return "Awe"; }
};
