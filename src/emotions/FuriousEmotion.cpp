#include "../include/emotions/FuriousEmotion.h"

void FuriousEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Furious();
  face->Behavior.Clear();
  face->Behavior.SetEmotion(eEmotions::Furious, 1.0);

  face->RandomBlink = false;
  face->RandomBehavior = false;
  face->RandomLook = false;

  face->Blink.Timer.SetIntervalMillis(7000);
}
