#include "../include/emotions/HappyEmotion.h"

void HappyEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Happy();
  face->Behavior.Clear();
  face->Behavior.SetEmotion(eEmotions::Happy, 1.0);

  face->RandomBlink = false;
  face->RandomBehavior = false;
  face->RandomLook = false;

  face->Blink.Timer.SetIntervalMillis(7000);
}
