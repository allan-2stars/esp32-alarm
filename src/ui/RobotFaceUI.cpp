#include "ui/RobotFaceUI.h"
#include "emotions/HappyEmotion.h"
#include "emotions/SurprisedEmotion.h"
#include "emotions/SadEmotion.h"
#include "emotions/AngryEmotion.h"
#include "emotions/AnnoyedEmotion.h"
#include "emotions/AweEmotion.h"
#include "emotions/FocusedEmotion.h"
#include "emotions/FrustratedEmotion.h"
#include "emotions/FuriousEmotion.h"
#include "emotions/GleeEmotion.h"
#include "emotions/NormalEmotion.h"
#include "emotions/ScaredEmotion.h"
#include "globals.h"
#include "config.h"

////// Aniamtion
#include "animations/FaceAnimation.h"


FaceAnimation* currentAnimation = nullptr;
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

  // Clear old animation if still active
  if (currentAnimation) {
    delete currentAnimation;
    currentAnimation = nullptr;
  }

  // Clear emotions list and reattach (only once ideally)
  if (emotionCount == 0) {
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
  }

  showEmotionByName("Normal");
  // drawEmotionLabel();
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

  // --- Handle active animation ---
  if (currentAnimation) {
  currentAnimation->update(millis());

  if (currentAnimation->isFinished()) {
    currentAnimation = nullptr;

    // ✅ Avoid flicker: only redraw if current emotion is not already Normal
    if (String(emotions[currentEmotionIndex]->getName()).equalsIgnoreCase("Normal") == false) {
      showEmotionByName("Normal");
      drawEmotionLabel();
    } else {
      Serial.println("⚠️ Already Normal — skipping redraw.");
    }
  }

  return;
}


  // --- (Optional) Other robot face updates here ---
  // e.g., blinking idle behavior, auto look, etc.
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

///////////// Animations /////////////
void RobotFaceUI::playGratefulAnimation() {
  if (!face) return;
  if (currentAnimation) return;  // Prevent interrupting current animation

  Serial.println("🎭 Playing Grateful Animation");
  gratefulAnimation.start(face);
  currentAnimation = &gratefulAnimation;
}
void RobotFaceUI::playSkepticAnimation() {
  if (!face) return;
  if (currentAnimation) return;

  Serial.println("🎭 Playing Skeptic Animation");
  skepticAnimation.start(face);
  currentAnimation = &skepticAnimation;
}

//////////////////////////////////////////////////

void RobotFaceUI::drawEmotionLabel() {
  display.setTextSize(1);
  display.setTextColor(TEXT_COLOR);
  display.setCursor(0, 0);

  String label = "Emotion: ";
  if (currentEmotionIndex >= 0 && currentEmotionIndex < emotionCount && emotions[currentEmotionIndex]) {
    label += emotions[currentEmotionIndex]->getName();
  } else {
    label += "Unknown";
  }

  display.fillRect(0, 0, SCREEN_WIDTH, 10, SSD1306_BLACK);  // clear top area
  display.print(label);
  display.display();
}

bool RobotFaceUI::isAnimationRunning() const {
  return currentAnimation && !currentAnimation->isFinished();
}
