#include "ui/RobotFaceUI.h"
#include "ui/emotions/HappyEmotion.h"
#include "ui/emotions/SurprisedEmotion.h"
#include "ui/emotions/SadEmotion.h"
#include "ui/emotions/AngryEmotion.h"
#include "ui/emotions/AnnoyedEmotion.h"
#include "ui/emotions/AweEmotion.h"
#include "ui/emotions/FocusedEmotion.h"
#include "ui/emotions/FrustratedEmotion.h"
#include "ui/emotions/FuriousEmotion.h"
#include "ui/emotions/GleeEmotion.h"
#include "ui/emotions/NormalEmotion.h"
#include "ui/emotions/ScaredEmotion.h"

// emotionCount tracks how many emotions were added via addEmotion()
// NUM_EMOTIONS is a fixed maximum capacity defined elsewhere (e.g., #define NUM_EMOTIONS 12)

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
  addEmotion(new AngryEmotion());
  addEmotion(new AnnoyedEmotion());
  addEmotion(new AweEmotion());
  addEmotion(new FocusedEmotion());
  addEmotion(new FrustratedEmotion());
  addEmotion(new FuriousEmotion());
  addEmotion(new GleeEmotion());
  addEmotion(new NormalEmotion());
  addEmotion(new ScaredEmotion());

  showEmotion(2); // default emotion (Sad)
  lastEmotionChange = millis();
}

void RobotFaceUI::showEmotion(int index) {
  if (index < 0 || index >= emotionCount || !emotions[index]) return;
  currentEmotionIndex = index;
  emotions[index]->activate();
}

void RobotFaceUI::reset() {
  currentEmotionIndex = 0;
  lastEmotionChange = millis();
  showEmotion(currentEmotionIndex);
}

void RobotFaceUI::update() {
  if (face) {
    face->Update();
  }
  //Serial.print("in Robot Face update...current Emotion Index: ");
  // Auto cycle every 3 seconds
  if (millis() - lastEmotionChange >= 3000) {
    currentEmotionIndex = (currentEmotionIndex + 1) % emotionCount;
    showEmotion(currentEmotionIndex);
    lastEmotionChange = millis();
  }
}

void RobotFaceUI::showEmotionByName(const String& name) {
  for (int i = 0; i < emotionCount; ++i) {
    if (String(emotions[i]->getName()).equalsIgnoreCase(name)) {
      showEmotion(i);
      return;
    }
  }

  Serial.print("Emotion not found: ");
  Serial.println(name);
}

void RobotFaceUI::playGratefulAnimation() {
  // Step 1: Awe for 1 second
  showEmotionByName("Awe");
  delay(1000);

  // Step 2: Blink
  face->DoBlink();
  delay(300); // Wait for blink to complete

  // Step 3: Happy for 2 seconds
  showEmotionByName("Happy");
  delay(2000);

  // Step 4: Blink twice
  face->DoBlink();
  delay(300);
  face->DoBlink();
  delay(300);

  // Step 5: Stay Happy for 2 seconds
  delay(2000);

  // Step 6: Return to Normal
  showEmotionByName("Normal");
}

