#include "../include/emotions/AnnoyedEmotion.h"

void AnnoyedEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Annoyed();
  face->Behavior.Clear();
  face->Behavior.SetEmotion(eEmotions::Annoyed, 1.0);

  face->RandomBlink = false;
  face->RandomBehavior = false;
  face->RandomLook = false;

  face->Blink.Timer.SetIntervalMillis(7000);
}
