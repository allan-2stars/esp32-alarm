#include "melodies.h"
#include "config.h"

void playNotes(const int* melody, int length, uint8_t tempo, int buzzerPin) {
  uint16_t wholenote = (60000 * 4) / tempo;
  for (int i = 0; i < length * 2; i += 2) {
    int divider = melody[i + 1];
    int noteDuration = (divider > 0) ? wholenote / divider : wholenote / abs(divider) * 1.5;
    if (melody[i] != REST)
      tone(buzzerPin, melody[i], noteDuration * 0.9);
    delay(noteDuration);
    noTone(buzzerPin);
  }
}

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

void weWishYou(uint8_t tempo, int buzzerPin) {
  playNotes(melodyWeWishYou, sizeof(melodyWeWishYou) / sizeof(melodyWeWishYou[0]) / 2, tempo, buzzerPin);
}
void whiteChristmas(uint8_t tempo, int buzzerPin) {
  playNotes(melodyWhiteChristmas, sizeof(melodyWhiteChristmas) / sizeof(melodyWhiteChristmas[0]) / 2, tempo, buzzerPin);
}
void jingleBells(uint8_t tempo, int buzzerPin) {
  playNotes(melodyJingleBells, sizeof(melodyJingleBells) / sizeof(melodyJingleBells[0]) / 2, tempo, buzzerPin);
}
void rudolfTheRedNosed(uint8_t tempo, int buzzerPin) {
  playNotes(melodyRudolfTheRedNosed, sizeof(melodyRudolfTheRedNosed) / sizeof(melodyRudolfTheRedNosed[0]) / 2, tempo, buzzerPin);
}
void santaClausIsComin(uint8_t tempo, int buzzerPin) {
  playNotes(melodySantaClausIsComin, sizeof(melodySantaClausIsComin) / sizeof(melodySantaClausIsComin[0]) / 2, tempo, buzzerPin);
}
void playSilentNight(uint8_t tempo, int buzzerPin) {
  playNotes(melodySilentNight, sizeof(melodySilentNight) / sizeof(melodySilentNight[0]) / 2, tempo, buzzerPin);
}

void playMelody(int id, int buzzerPin) {
  switch (id) {
    case 0: weWishYou(160, buzzerPin); break;
    case 1: whiteChristmas(155, buzzerPin); break;
    case 2: jingleBells(180, buzzerPin); break;
    case 3: rudolfTheRedNosed(150, buzzerPin); break;
    case 4: santaClausIsComin(137, buzzerPin); break;
    case 5: playSilentNight(130, buzzerPin); break;
    default:
      tone(buzzerPin, NOTE_C5, 300); delay(300);
      noTone(buzzerPin); break;
  }
}



