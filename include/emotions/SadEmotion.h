#pragma once
#include "FaceEmotion.h"

class SadEmotion : public FaceEmotion {
public:
  void activate() override;
  const char* getName() const override { return "Sad"; }
};
