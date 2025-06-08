// src/animations/GratefulAnimation.cpp
#include "../include/animations/GreatfulAnimation.h"
#include "Face.h"

void GratefulAnimation::start(Face* f) {
  face = f;
  step = 0;
  stepStart = millis();
  finished = false;
  face->Expression.GoTo_Awe();
}

void GratefulAnimation::update(unsigned long now) {
  if (finished || !face) return;

  switch (step) {
    case 0:
      if (now - stepStart > 1000) {
        face->DoBlink();
        step = 1;
        stepStart = now;
      }
      break;
    case 1:
      if (now - stepStart > 200) {
        face->Expression.GoTo_Happy();
        step = 2;
        stepStart = now;
      }
      break;
    case 2:
      if (now - stepStart > 2000) {
        face->DoBlink();
        step = 3;
        stepStart = now;
      }
      break;
    case 3:
      if (now - stepStart > 200) {
        face->DoBlink();
        step = 4;
        stepStart = now;
      }
      break;
    case 4:
      if (now - stepStart > 200) {
        face->Expression.GoTo_Happy();
        step = 5;
        stepStart = now;
      }
      break;
    case 5:
      if (now - stepStart > 2000) {
        finished = true;
      }
      break;
  }
}

bool GratefulAnimation::isFinished() const {
  return finished;
}
