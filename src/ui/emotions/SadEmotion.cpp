#include "ui/emotions/SadEmotion.h"

void SadEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Sad();
  face->Behavior.Clear();
  face->Behavior.SetEmotion(eEmotions::Sad, 1.0);

  face->RandomBlink = false;
  face->RandomBehavior = false;
  face->RandomLook = false;

  face->Blink.Timer.SetIntervalMillis(7000);
}
