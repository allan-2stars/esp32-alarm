#pragma once
#include "FaceEmotion.h"

class GleeEmotion : public FaceEmotion {
public:
  void activate() override;
  const char* getName() const override { return "Glee"; }
};
