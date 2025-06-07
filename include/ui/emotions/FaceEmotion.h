#pragma once

#include <Face.h>

// Abstract base class for robot face emotions
class FaceEmotion {
public:
  virtual ~FaceEmotion() = default;

  // Link the face instance to the emotion
  void attach(Face* f) {
    face = f;
  }

  // Called when the emotion becomes active (must override)
  virtual void activate() = 0;

protected:
  Face* face = nullptr;  // Set by `attach()` before activate() is called
};
