#include "ui/RobotFaceUI.h"
#include "ui/emotions/HappyEmotion.h"
#include "ui/emotions/SurprisedEmotion.h" // Add other emotions similarly
#include "ui/emotions/SadEmotion.h"

// emotionCount tracks how many emotions were added via addEmotion()
// NUM_EMOTIONS is a fixed maximum capacity defined elsewhere (e.g., #define NUM_EMOTIONS 10)

RobotFaceUI::RobotFaceUI() : emotionCount(0), currentEmotionIndex(0), lastEmotionChange(0) {}

RobotFaceUI::~RobotFaceUI() {
  for (int i = 0; i < emotionCount; ++i) {
    delete emotions[i];
  }
  delete face;
}

// As soon as a RobotFaceUI object is created, emotionCount starts at 0.
// Each call to addEmotion(...) increments it by one.
// This counter is used to track how many emotions have been added dynamically to the emotions[] array.
void RobotFaceUI::addEmotion(FaceEmotion* emotion) {
  if (emotionCount < NUM_EMOTIONS) {
    emotions[emotionCount++] = emotion;
    emotion->attach(face);
  }
}

void RobotFaceUI::begin() {
  if (!face) {
    face = new Face(128, 64, 40);  // Width, Height, Eye Size
    face->RandomBlink = true;
    face->RandomBehavior = false;
    face->RandomLook = false;
    face->Blink.Timer.SetIntervalMillis(5000);
  }

  emotionCount = 0;  // reset count
  addEmotion(new HappyEmotion());
  addEmotion(new SurprisedEmotion());
  addEmotion(new SadEmotion());
  // Add more: addEmotion(new AngryEmotion()); etc.

  showEmotion(2); // default emotion
}

void RobotFaceUI::reset() {
  currentEmotionIndex = 0;
  lastEmotionChange = millis();
  showEmotion(currentEmotionIndex);
}

void RobotFaceUI::showEmotion(int index) {
  if (index < 0 || index >= emotionCount || !emotions[index]) return;
  currentEmotionIndex = index;
  emotions[index]->activate();
}

void RobotFaceUI::update() {
  if (face) {
    face->Update();
  }
}
