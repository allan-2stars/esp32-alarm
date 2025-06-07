#include "ui/emotions/FrustratedEmotion.h"

void FrustratedEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Frustrated();             // Uses frustrated expression
  face->Behavior.Clear();
  face->Behavior.SetEmotion(eEmotions::Frustrated, 1.0);  // Full frustration expression

  face->RandomBlink = false;        // Rigid look
  face->RandomBehavior = false;
  face->RandomLook = false;

  face->Blink.Timer.SetIntervalMillis(7000);  // Rare blinking to enhance frustration
}
