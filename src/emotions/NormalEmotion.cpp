#include "../include/emotions/NormalEmotion.h"

void NormalEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Normal();
  face->Behavior.Clear();
  face->Behavior.SetEmotion(eEmotions::Normal, 1.0);

  face->RandomBlink = false;
  face->RandomBehavior = false;
  face->RandomLook = false;

  face->Blink.Timer.SetIntervalMillis(7000);
}
