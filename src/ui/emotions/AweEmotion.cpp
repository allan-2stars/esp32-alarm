#include "ui/emotions/AweEmotion.h"

void AweEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Awe();
  face->Behavior.Clear();
  face->Behavior.SetEmotion(eEmotions::Awe, 1.0);

  face->RandomBlink = false;
  face->RandomBehavior = false;
  face->RandomLook = false;

  face->Blink.Timer.SetIntervalMillis(7000);
}
