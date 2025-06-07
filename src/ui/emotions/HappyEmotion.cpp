#include "ui/emotions/HappyEmotion.h"

void HappyEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Happy();           // Set happy expression
  face->Behavior.Clear();                  // Clear previous emotional behavior
  face->Behavior.SetEmotion(eEmotions::Happy, 1.0);  // Full happy weight
  face->RandomBlink = true;
  face->RandomBehavior = false;
  face->RandomLook = false;

  // Optional: adjust blink rate
  face->Blink.Timer.SetIntervalMillis(3000);
}
