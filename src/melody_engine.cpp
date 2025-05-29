#include "melodies.h"
#include "melody_engine.h"

static const int* currentMelody = nullptr;
static int melodyLength = 0;
static int melodyTempo = 120;
static int melodyIndex = 0;
static unsigned long lastNoteTime = 0;
static int buzzerPin = 0;
static bool melodyPlaying = false;
static bool looping = false;


  // void startMelodyPreview(const int* melody, int length, int tempo, int pin) {
  //   currentMelody = melody;
  //   melodyLength = length;
  //   melodyTempo = tempo;
  //   melodyIndex = 0;
  //   lastNoteTime = 0;
  //   buzzerPin = pin;
  //   melodyPlaying = true;
  // }

void startMelody(const int* melody, int length, int tempo, int pin, bool loop) {
  currentMelody = melody;
  melodyLength = length;
  melodyTempo = tempo;
  melodyIndex = 0;
  lastNoteTime = 0;
  buzzerPin = pin;
  melodyPlaying = true;
  looping = loop;
}

void updateMelodyPlayback() {
  if (!melodyPlaying || !currentMelody) {
    return;  // nothing to do
  }

  unsigned long now = millis();
  if (now >= lastNoteTime) {
    int note = currentMelody[melodyIndex * 2];
    int duration = currentMelody[melodyIndex * 2 + 1];
    int wholenote = (60000 * 4) / melodyTempo;
    int noteDuration = (duration > 0) ? wholenote / duration : (wholenote / abs(duration)) * 1.5;

    if (note > 0) {
      tone(buzzerPin, note, noteDuration * 0.9);  // play tone
    } else {
      noTone(buzzerPin);  // rest
    }

    lastNoteTime = now + noteDuration;
    melodyIndex++;

    // 🔁 Loop or stop when done
    if (melodyIndex >= melodyLength) {
      if (looping) {
        melodyIndex = 0;
      } else {
        melodyPlaying = false;
        noTone(buzzerPin);
      }
    }
  }
}


bool isMelodyPlaying() {
  return melodyPlaying;
}

void stopMelody() {
  noTone(buzzerPin);
  melodyPlaying = false;
  looping = false;  // Important: turn off loop mode
  Serial.println("Stop melody");
}
