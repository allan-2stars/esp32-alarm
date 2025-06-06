#include "services/MelodyService.h"
#include <Arduino.h>
//#include "melody_engine.h"

void MelodyService::begin() {
  stop();
}

void MelodyService::play(const int* melody, int length, int tempo, int pin, bool loop) {
  currentMelody = melody;
  melodyLength = length;
  melodyTempo = tempo;
  melodyIndex = 0;
  lastNoteTime = 0;
  buzzerPin = pin;
  playing = true;
  looping = loop;
}

void MelodyService::update() {
  if (!playing || !currentMelody) return;

  unsigned long now = millis();
  if (now >= lastNoteTime) {
    int note = currentMelody[melodyIndex * 2];
    int duration = currentMelody[melodyIndex * 2 + 1];
    int wholenote = (60000 * 4) / melodyTempo;
    int noteDuration = (duration > 0) ? wholenote / duration : (wholenote / abs(duration)) * 1.5;

    if (note > 0) {
      tone(buzzerPin, note, noteDuration * 0.9);
    } else {
      noTone(buzzerPin);
    }

    lastNoteTime = now + noteDuration;
    melodyIndex++;

    if (melodyIndex >= melodyLength) {
      if (looping) {
        melodyIndex = 0;
      } else {
        stop();
      }
    }
  }
}

void MelodyService::stop() {
  playing = false;
  noTone(buzzerPin);
}

bool MelodyService::isPlaying() const {
  return playing;
}