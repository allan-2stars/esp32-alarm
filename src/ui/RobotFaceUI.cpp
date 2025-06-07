#include "ui/RobotFaceUI.h"
#include "ui/emotions/HappyEmotion.h"

RobotFaceUI::RobotFaceUI() : face(nullptr), currentEmotionIndex(0), lastEmotionChange(0) {}

RobotFaceUI::~RobotFaceUI() {
  for (int i = 0; i < NUM_EMOTIONS; ++i) {
    delete emotions[i];
  }
  delete face;
}

void RobotFaceUI::begin() {
  if (!face) {
    face = new Face(128, 64, 40);  // Width, Height, Eye Size
    face->RandomBlink = true;
    face->RandomBehavior = false;
    face->RandomLook = false;
    face->Blink.Timer.SetIntervalMillis(5000);
  }

  // Initialize emotion list
  emotions[0] = new HappyEmotion();
  for (int i = 0; i < NUM_EMOTIONS; ++i) {
    emotions[i]->attach(face);
  }

  showEmotion(0);
}

void RobotFaceUI::reset() {
  currentEmotionIndex = 0;
  lastEmotionChange = millis();
  showEmotion(currentEmotionIndex);
}

void RobotFaceUI::showEmotion(int index) {
  if (index < 0 || index >= NUM_EMOTIONS) return;
  currentEmotionIndex = index;
  emotions[index]->activate();
}

void RobotFaceUI::update() {
  if (face) {
    face->Update();
  }
}
