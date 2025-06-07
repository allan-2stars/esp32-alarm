#include "ui/emotions/SurprisedEmotion.h"

void SurprisedEmotion::activate() {
  if (!face) return;

  face->Expression.GoTo_Surprised();            // Switch to surprised expression
  face->Behavior.Clear();                       // Reset all behavior
  face->Behavior.SetEmotion(eEmotions::Surprised, 0.5);  // Full surprised emotion 0.1-1.0 is the possibility

  face->RandomBlink = false;  // Hold stare
  face->RandomBehavior = false;
  face->RandomLook = false;

  // Optional: slower or frozen blink
  face->Blink.Timer.SetIntervalMillis(10000);
}
