#include "ui/emotions/ScaredEmotion.h"

void ScaredEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Scared();
  face->Behavior.Clear();
  face->Behavior.SetEmotion(eEmotions::Scared, 1.0);

  face->RandomBlink = false;
  face->RandomBehavior = false;
  face->RandomLook = false;

  face->Blink.Timer.SetIntervalMillis(7000);
}
