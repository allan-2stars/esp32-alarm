#include "ui/RobotFaceUI.h"
#include "ui/emotions/EmotionHappy.h"
//#include "ui/emotions/EmotionAngry.h"

EmotionHappy happy;
//EmotionAngry angry;

Emotion* robotEmotions[] = {
  &happy,
 // &angry
};

const int NUM_EMOTIONS = sizeof(robotEmotions) / sizeof(robotEmotions[0]);

RobotFaceUI::RobotFaceUI(Adafruit_SSD1306& disp) : display(disp), currentEmotion(0), lastChange(0) {}

RobotFaceUI::~RobotFaceUI() {
  // Nothing to delete when using static emotion instances
}

void RobotFaceUI::reset() {
  currentEmotion = 0;
  lastChange = millis();
}

void RobotFaceUI::update() {
  if (millis() - lastChange > 3000) {
    currentEmotion = (currentEmotion + 1) % NUM_EMOTIONS;
    lastChange = millis();
  }

  display.clearDisplay();
  robotEmotions[currentEmotion]->draw(display);
  display.display();
}