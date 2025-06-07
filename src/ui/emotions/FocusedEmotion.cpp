#include "ui/emotions/FocusedEmotion.h"

void FocusedEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Focused();
  face->Behavior.Clear();
  face->Behavior.SetEmotion(eEmotions::Focused, 1.0);

  face->RandomBlink = false;
  face->RandomBehavior = false;
  face->RandomLook = false;

  face->Blink.Timer.SetIntervalMillis(7000);
}
