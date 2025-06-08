#pragma once
#include "FaceEmotion.h"

class AnnoyedEmotion : public FaceEmotion {
public:
  void activate() override;
  const char* getName() const override { return "Annoyed"; }
};
