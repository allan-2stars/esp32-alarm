#include "ui/emotions/GleeEmotion.h"

void GleeEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Glee();
  face->Behavior.Clear();
  face->Behavior.SetEmotion(eEmotions::Glee, 1.0);

  face->RandomBlink = false;
  face->RandomBehavior = false;
  face->RandomLook = false;

  face->Blink.Timer.SetIntervalMillis(7000);
}
