#pragma once

#include <Face.h>
#include "emotions/FaceEmotion.h"

#define NUM_EMOTIONS 12  // Expandable later

class RobotFaceUI {
public:
  RobotFaceUI();
  ~RobotFaceUI();

  void begin();
  void update();
  void reset();
  void playGratefulAnimation();
  void playSkepticAnimation();
  
private:
  Face* face;
  FaceEmotion* emotions[NUM_EMOTIONS];
  int currentEmotionIndex = 0;
  unsigned long lastEmotionChange = 0;
  int emotionCount = 0;
  void showEmotion(int index);  // ← define this method
  void addEmotion(FaceEmotion* emotion);

  void showEmotionByName(const String& name);
  unsigned long returnToNormalAt = 0;  // ⏱ 0 = no scheduled return
  void drawEmotionLabel();
  bool isAnimating = false;

};
