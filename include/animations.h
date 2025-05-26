#ifndef ANIMATIONS_H
#define ANIMATIONS_H

#include <Arduino.h>

void setupAnimations();         // Optional, if needed later
void updateAnimations();        // Call from loop()
void updateMoonLedFade(bool);   // Exposed for manual control if needed

#endif
