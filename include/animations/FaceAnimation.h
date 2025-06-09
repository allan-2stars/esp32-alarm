#pragma once
class Face;

class FaceAnimation {
public:
  virtual void start(Face* face) = 0;
  virtual void update(unsigned long now) = 0;
  virtual bool isFinished() const = 0;
  virtual ~FaceAnimation() = default;
};
