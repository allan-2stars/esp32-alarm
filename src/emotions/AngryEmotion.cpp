#include "../include/emotions/AngryEmotion.h"

void AngryEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Angry();
  face->Behavior.Clear();
  face->Behavior.SetEmotion(eEmotions::Angry, 1.0);

  face->RandomBlink = false;
  face->RandomBehavior = false;
  face->RandomLook = false;

  face->Blink.Timer.SetIntervalMillis(7000);
}
