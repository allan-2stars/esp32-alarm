#pragma once

#include <Face.h>
#include "ui/emotions/FaceEmotion.h"

#define NUM_EMOTIONS 1  // Expandable later

class RobotFaceUI {
public:
  RobotFaceUI();
  ~RobotFaceUI();

  void begin();
  void update();
  void reset();

private:
  Face* face;
  FaceEmotion* emotions[NUM_EMOTIONS];
  int currentEmotionIndex = 0;
  unsigned long lastEmotionChange = 0;

  void showEmotion(int index);  // ← define this method
};
