#include "melodies.h"
#include "config.h"


const int melodyWeWishYou[] = {
  NOTE_D4, 4, NOTE_G4, 4, NOTE_G4, 4, NOTE_A4, 4,
  NOTE_G4, 4, NOTE_FS4, 4, NOTE_E4, 2,
  NOTE_E4, 4, NOTE_A4, 4, NOTE_A4, 4, NOTE_B4, 4,
  NOTE_A4, 4, NOTE_G4, 4, NOTE_FS4, 2,
  NOTE_D4, 4, NOTE_B4, 4, NOTE_B4, 4, NOTE_C5, 4,
  NOTE_B4, 4, NOTE_A4, 4, NOTE_G4, 2,
  NOTE_E4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_FS4, 4,
  NOTE_G4, 2, NOTE_D4, 2
};

extern const int melodyWhiteChristmas[] = {
  NOTE_D4, 4, NOTE_G4, 4, NOTE_G4, 4, NOTE_A4, 4,
  NOTE_G4, 4, NOTE_FS4, 4, NOTE_E4, 2,
  NOTE_E4, 4, NOTE_A4, 4, NOTE_A4, 4, NOTE_B4, 4,
  NOTE_A4, 4, NOTE_G4, 4, NOTE_FS4, 2,
  NOTE_D4, 4, NOTE_B4, 4, NOTE_B4, 4, NOTE_C5, 4,
  NOTE_B4, 4, NOTE_A4, 4, NOTE_G4, 2,
  NOTE_E4, 4, NOTE_E4, 4, NOTE_A4, 4, NOTE_FS4, 4,
  NOTE_G4, 2, NOTE_D4, 2
};

extern const int melodyJingleBells[] = {
  NOTE_E4, 4, NOTE_E4, 4, NOTE_E4, 2,
  NOTE_E4, 4, NOTE_E4, 4, NOTE_E4, 2,
  NOTE_E4, 4, NOTE_G4, 4, NOTE_C4, 4, NOTE_D4, 4,
  NOTE_E4, 1,

  NOTE_F4, 4, NOTE_F4, 4, NOTE_F4, 4,
  NOTE_F4, 4, NOTE_F4, 4, NOTE_E4, 4, NOTE_E4, 4,
  NOTE_E4, 4, NOTE_E4, 4, NOTE_D4, 4, NOTE_D4, 4,
  NOTE_E4, 4, NOTE_D4, 2, NOTE_G4, 2
};

extern const int melodyRudolfTheRedNosed[] = {
  NOTE_G4, 4, NOTE_E4, 4, NOTE_F4, 4, NOTE_G4, 4,
  NOTE_E4, 2, NOTE_G4, 4, NOTE_E4, 4, NOTE_F4, 4,
  NOTE_G4, 4, NOTE_E4, 2, NOTE_G4, 4, NOTE_E4, 4,
  NOTE_F4, 4, NOTE_E4, 4, NOTE_D4, 2, NOTE_C4, 2,

  NOTE_C4, 4, NOTE_D4, 4, NOTE_E4, 4, NOTE_F4, 4,
  NOTE_G4, 2, NOTE_A4, 4, NOTE_B4, 4, NOTE_C5, 4,
  NOTE_D5, 4, NOTE_E5, 2, NOTE_F5, 4, NOTE_E5, 4,
  NOTE_D5, 4, NOTE_C5, 4, NOTE_B4, 2, NOTE_A4, 2
};

extern const int melodySantaClausIsComin[] = {
  NOTE_G4,8,
  NOTE_E4,8, NOTE_F4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4,
  NOTE_A4,8, NOTE_B4,8, NOTE_C5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_E4,8, NOTE_F4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4,
  NOTE_A4,8, NOTE_G4,8, NOTE_F4,4, NOTE_F4,2,
  NOTE_E4,4, NOTE_G4,4, NOTE_C4,4, NOTE_E4,4,
  NOTE_D4,4, NOTE_F4,2, NOTE_B3,4,
  NOTE_C4,-2, REST,4,
  NOTE_G4,8,
  NOTE_E4,8, NOTE_F4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4,
  NOTE_A4,8, NOTE_B4,8, NOTE_C5,4, NOTE_C5,4, NOTE_C5,4,
  NOTE_E4,8, NOTE_F4,8, NOTE_G4,4, NOTE_G4,4, NOTE_G4,4,
  NOTE_A4,8, NOTE_G4,8, NOTE_F4,4, NOTE_F4,2,
  NOTE_E4,4, NOTE_G4,4, NOTE_C4,4, NOTE_E4,4,
  NOTE_D4,4, NOTE_F4,2, NOTE_D5,4,
  NOTE_C5,1
};

extern const int melodySilentNight[] = {
  NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
  NOTE_E4,-2, 
  NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
  NOTE_E4,-2, 
  NOTE_D5,2, NOTE_D5,4,
  NOTE_B4,-2,
  NOTE_C5,2, NOTE_C5,4,
  NOTE_G4,-2,
  NOTE_A4,2, NOTE_A4,4,
  NOTE_C5,-4, NOTE_B4,8, NOTE_A4,4,
  NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
  NOTE_E4,-2, 
  NOTE_A4,2, NOTE_A4,4,
  NOTE_C5,-4, NOTE_B4,8, NOTE_A4,4,
  NOTE_G4,-4, NOTE_A4,8, NOTE_G4,4,
  NOTE_E4,-2, 
  NOTE_D5,2, NOTE_D5,4,
  NOTE_F5,-4, NOTE_D5,8, NOTE_B4,4,
  NOTE_C5,-2,
  NOTE_E5,-2,
  NOTE_C5,4, NOTE_G4,4, NOTE_E4,4,
  NOTE_G4,-4, NOTE_F4,8, NOTE_D4,4,
  NOTE_C4,-2
};


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


const int* getMelodyData(int id) {
  switch (id) {
    case 0: return melodyWeWishYou;
    case 1: return melodyWhiteChristmas;
    case 2: return melodyJingleBells;
    case 3: return melodyRudolfTheRedNosed;
    case 4: return melodySantaClausIsComin;
    case 5: return melodySilentNight;
    default: return nullptr;
  }
}

int getMelodyLength(int id) {
  switch (id) {
    case 0: return sizeof(melodyWeWishYou) / sizeof(melodyWeWishYou[0]) / 2;
    case 1: return sizeof(melodyWhiteChristmas) / sizeof(melodyWhiteChristmas[0]) / 2;
    case 2: return sizeof(melodyJingleBells) / sizeof(melodyJingleBells[0]) / 2;
    case 3: return sizeof(melodyRudolfTheRedNosed) / sizeof(melodyRudolfTheRedNosed[0]) / 2;
    case 4: return sizeof(melodySantaClausIsComin) / sizeof(melodySantaClausIsComin[0]) / 2;
    case 5: return sizeof(melodySilentNight) / sizeof(melodySilentNight[0]) / 2;
    default: return 0;
  }
}

int getMelodyTempo(int id) {
  switch (id) {
    case 0: return 160;
    case 1: return 155;
    case 2: return 180;
    case 3: return 150;
    case 4: return 137;
    case 5: return 130;
    default: return 120;
  }
}

void stopMelody() {
  melodyPlaying = false;
  noTone(buzzerPin);
  Serial.println("Stop melody");
}


