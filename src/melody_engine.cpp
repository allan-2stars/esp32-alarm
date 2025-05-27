#include "melodies.h"
#include "melody_engine.h"

static const int* currentMelody = nullptr;
static int melodyLength = 0;
static int melodyTempo = 120;
static int melodyIndex = 0;
static unsigned long lastNoteTime = 0;
static int buzzerPin = 0;
static bool melodyPlaying = false;

void startMelodyPreview(const int* melody, int length, int tempo, int pin) {
  currentMelody = melody;
  melodyLength = length;
  melodyTempo = tempo;
  melodyIndex = 0;
  lastNoteTime = 0;
  buzzerPin = pin;
  melodyPlaying = true;
}

void updateMelodyPlayback() {
  if (!melodyPlaying || !currentMelody) {
    return;  // nothing to do
  }

  unsigned long now = millis();
  if (now >= lastNoteTime) { //It advances the melody based on time
    int note = currentMelody[melodyIndex * 2];
    int duration = currentMelody[melodyIndex * 2 + 1];
    int wholenote = (60000 * 4) / melodyTempo;
    int noteDuration = (duration > 0) ? wholenote / duration : (wholenote / abs(duration)) * 1.5;

    if (note > 0) {
      tone(buzzerPin, note, noteDuration * 0.9);  // play tone for ~90% duration
    } else {
      noTone(buzzerPin);  // rest
    }

    lastNoteTime = now + noteDuration;
    melodyIndex++;

    if (melodyIndex >= melodyLength) {
      melodyPlaying = false;
      noTone(buzzerPin);  // stop at end of melody
    }
  }
}

bool isMelodyPlaying() {
  return melodyPlaying;
}

void stopMelody() {
  melodyPlaying = false;
  noTone(buzzerPin);
  Serial.println("Stop melody");
}
