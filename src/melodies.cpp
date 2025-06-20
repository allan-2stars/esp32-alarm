
#include "melodies.h"
#include "Arduino.h"

#define NOTE_B0    31
#define NOTE_C1    33
#define NOTE_CS1   35
#define NOTE_D1    37
#define NOTE_DS1   39
#define NOTE_E1    41
#define NOTE_F1    44
#define NOTE_FS1   46
#define NOTE_G1    49
#define NOTE_GS1   52
#define NOTE_A1    55
#define NOTE_AS1   58
#define NOTE_B1    62
#define NOTE_C2    65
#define NOTE_CS2   69
#define NOTE_D2    73
#define NOTE_DS2   78
#define NOTE_E2    82
#define NOTE_F2    87
#define NOTE_FS2   93
#define NOTE_G2    98
#define NOTE_GS2  104
#define NOTE_A2   110
#define NOTE_AS2  117
#define NOTE_B2   123
#define NOTE_C3   131
#define NOTE_CS3  139
#define NOTE_D3   147
#define NOTE_DS3  156
#define NOTE_E3   165
#define NOTE_F3   175
#define NOTE_FS3  185
#define NOTE_G3   196
#define NOTE_GS3  208
#define NOTE_A3   220
#define NOTE_AS3  233
#define NOTE_B3   247
#define NOTE_C4   262
#define NOTE_CS4  277
#define NOTE_D4   294
#define NOTE_DS4  311
#define NOTE_E4   330
#define NOTE_F4   349
#define NOTE_FS4  370
#define NOTE_G4   392
#define NOTE_GS4  415
#define NOTE_A4   440
#define NOTE_AS4  466
#define NOTE_B4   494
#define NOTE_C5   523
#define NOTE_CS5  554
#define NOTE_D5   587
#define NOTE_DS5  622
#define NOTE_E5   659
#define NOTE_F5   698
#define NOTE_FS5  740
#define NOTE_G5   784
#define NOTE_GS5  831
#define NOTE_A5   880
#define NOTE_AS5  932
#define NOTE_B5   988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST        0

const int MELODY_COUNT = 7;
const char* melodyNames[MELODY_COUNT] = {
  "We Wish You", "White Xmas", "Jingle Bell", "Rudolf Red Nosed", "Santa Coming", "Silent Night", "We Three Kings"
};

const int melodyWeWishYou[] = {
    NOTE_C5,4,
    NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
    NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
    NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
    NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
    NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
    NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
    NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
  
    NOTE_F5,2, NOTE_C5,4,
    NOTE_F5,4, NOTE_F5,8, NOTE_G5,8, NOTE_F5,8, NOTE_E5,8,
    NOTE_D5,4, NOTE_D5,4, NOTE_D5,4,
    NOTE_G5,4, NOTE_G5,8, NOTE_A5,8, NOTE_G5,8, NOTE_F5,8,
    NOTE_E5,4, NOTE_C5,4, NOTE_C5,4,
    NOTE_A5,4, NOTE_A5,8, NOTE_AS5,8, NOTE_A5,8, NOTE_G5,8,
    NOTE_F5,4, NOTE_D5,4, NOTE_C5,8, NOTE_C5,8,
    NOTE_D5,4, NOTE_G5,4, NOTE_E5,4,
    NOTE_F5,2
};
const int melodyWeWishYouLength = sizeof(melodyWeWishYou) / sizeof(melodyWeWishYou[0]) / 2;

extern const int melodyWhiteChristmas[] = {
    NOTE_B4,1, NOTE_C5,4, NOTE_B4,4, 
    NOTE_AS4,4, NOTE_B4,4, NOTE_C5,1, NOTE_CS5,4, 
    NOTE_D5,-2, REST,16, NOTE_E5,4, NOTE_FS5,4, NOTE_G5,4, 
    NOTE_A5,4, NOTE_G5,4, NOTE_FS5,4, NOTE_E5,4,
    NOTE_D5,1, REST,8, NOTE_G4,4, NOTE_A4,4,
    NOTE_B4,2, NOTE_B4,2, NOTE_B4,4, NOTE_E5,2,
    NOTE_D5,4, NOTE_G4,2, NOTE_G4,2, NOTE_G4,4,
    NOTE_D5,2, NOTE_C5,4, NOTE_B4,1, NOTE_C5,4, 
    NOTE_B4,4, NOTE_A4,4, NOTE_G4,4, NOTE_A4,-1,
    NOTE_B4,1, NOTE_C5,4, NOTE_B4,4, NOTE_AS4,4, 
    NOTE_B4,4, NOTE_C5,1, NOTE_CS5,4, NOTE_D5,-2, 
    REST,16, NOTE_E5,4, NOTE_FS5,4, NOTE_G5,4, 
    NOTE_A5,4, NOTE_G5,4, NOTE_FS5,4, NOTE_E5,4, 
    NOTE_D5,1, REST,8, NOTE_G4,4, NOTE_A4,4,
    NOTE_B4,2, NOTE_B4,2, NOTE_B4,4, NOTE_E5,2, 
    NOTE_D5,4, NOTE_G5,1, REST,16, NOTE_G4,4, 
    NOTE_A4,4, NOTE_B4,2, NOTE_B4,2, NOTE_E5,4, 
    NOTE_E5,4, NOTE_FS4,4, NOTE_FS4,4, NOTE_G4,-1
};
const int melodyWhiteChristmasLength = sizeof(melodyWhiteChristmas) / sizeof(melodyWhiteChristmas[0]) / 2;

extern const int melodyJingleBells[] = {
    NOTE_E5,4, NOTE_E5,4, NOTE_E5,2,
    NOTE_E5,4, NOTE_E5,4, NOTE_E5,2,
    NOTE_E5,4, NOTE_G5,4, NOTE_C5,4, NOTE_D5,4,
    NOTE_E5,-2, REST,8,
    NOTE_F5,4, NOTE_F5,4, NOTE_F5,-4, NOTE_F5,8,
    NOTE_F5,4, NOTE_E5,4, NOTE_E5,4, NOTE_E5,8, NOTE_E5,8,
    NOTE_E5,4, NOTE_D5,4, NOTE_D5,4, NOTE_E5,4,
    NOTE_D5,2, NOTE_G5,2,
    NOTE_E5,4, NOTE_E5,4, NOTE_E5,2,
    NOTE_E5,4, NOTE_E5,4, NOTE_E5,2,
    NOTE_E5,4, NOTE_G5,4, NOTE_C5,4, NOTE_D5,4,
    NOTE_E5,-2, REST,8,
    NOTE_F5,4, NOTE_F5,4, NOTE_F5,4, NOTE_F5,4,
    NOTE_F5,4, NOTE_E5,4, NOTE_E5,4, NOTE_E5,8, NOTE_E5,8,
    NOTE_G5,4, NOTE_G5,4, NOTE_F5,4, NOTE_D5,4,
    NOTE_C5,-2
};
const int melodyJingleBellsLength = sizeof(melodyJingleBells) / sizeof(melodyJingleBells[0]) / 2;

extern const int melodyRudolfTheRedNosed[] = {
    NOTE_G4,8, NOTE_A4,4, NOTE_G4,8, NOTE_E4,4, NOTE_C5,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_C5,4, NOTE_B4,1,
    NOTE_F4,8, NOTE_G4,4, NOTE_F4,8, NOTE_D4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_A4,4, NOTE_E4,1,
    NOTE_G4,8, NOTE_A4,4, NOTE_G4,8, NOTE_E4,4, NOTE_C5,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_C5,4, NOTE_B4,1,
    NOTE_F4,8, NOTE_G4,4, NOTE_F4,8, NOTE_D4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_D5,4, NOTE_C5,1,
    NOTE_A4,4, NOTE_A4,4, NOTE_C5,4, NOTE_A4,4, NOTE_G4,4, NOTE_E4,4, NOTE_G4,2, NOTE_F4,4, NOTE_A4,4, NOTE_G4,4, NOTE_F4,4, NOTE_E4,1,
    NOTE_D4,4, NOTE_E4,4, NOTE_G4,4, NOTE_A4,4, NOTE_B4,4, NOTE_B4,4, NOTE_B4,2, NOTE_C5,4, NOTE_C5,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,4, NOTE_F4,8, NOTE_D4,-2,
    NOTE_G4,8, NOTE_A4,4, NOTE_G4,8, NOTE_E4,4, NOTE_C5,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_C5,4, NOTE_B4,1,
    NOTE_F4,8, NOTE_G4,4, NOTE_F4,8, NOTE_D4,4, NOTE_B4,4, NOTE_A4,4, NOTE_G4,-2, NOTE_G4,8, NOTE_A4,8, NOTE_G4,8, NOTE_A4,8, NOTE_G4,4, NOTE_D5,4, NOTE_C5,1
};
const int melodyRudolfTheRedNosedLength = sizeof(melodyRudolfTheRedNosed) / sizeof(melodyRudolfTheRedNosed[0]) / 2;

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
const int melodySantaClausIsCominLength = sizeof(melodySantaClausIsComin) / sizeof(melodySantaClausIsComin[0]) / 2;

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
const int melodySilentNightLength = sizeof(melodySilentNight) / sizeof(melodySilentNight[0]) / 2;

extern const int melodyWeThreeKings[] = {
    NOTE_B4,2, NOTE_A4,4, NOTE_G4,2, NOTE_E4,4, 
    NOTE_FS4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_E4,-2,
    NOTE_B4,2, NOTE_A4,4, NOTE_G4,2, NOTE_E4,4, 
    NOTE_FS4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_E4,-2,
    NOTE_G4,2, NOTE_G4,4, NOTE_A4,2, NOTE_A4,4, 
    NOTE_B4,2, NOTE_B4,4, NOTE_D5,4, NOTE_C5,4, 
    NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_A4,4, 
    NOTE_G4,2, NOTE_FS4,4, NOTE_E4,-2, REST,4,
    NOTE_B4,2, NOTE_A4,4, NOTE_G4,2, NOTE_E4,4, 
    NOTE_FS4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_E4,-2,
    NOTE_B4,2, NOTE_A4,4, NOTE_G4,2, NOTE_E4,4, 
    NOTE_FS4,4, NOTE_G4,4, NOTE_FS4,4, NOTE_E4,-2,
    NOTE_G4,2, NOTE_G4,4, NOTE_A4,2, NOTE_A4,4, 
    NOTE_B4,2, NOTE_B4,4, NOTE_D5,4, NOTE_C5,4, 
    NOTE_B4,4, NOTE_A4,4, NOTE_B4,4, NOTE_A4,4, 
    NOTE_G4,2, NOTE_FS4,4, NOTE_E4,1
};
const int melodyWeThreeKingsLength = sizeof(melodyWeThreeKings) / sizeof(melodyWeThreeKings[0]) / 2;


const int* melodyDataList[MELODY_COUNT] = {
  melodyWeWishYou,
  melodyWhiteChristmas,
  melodyJingleBells,
  melodyRudolfTheRedNosed,
  melodySantaClausIsComin,
  melodySilentNight,
  melodyWeThreeKings
};

int melodyLengthList[MELODY_COUNT] = {
  melodyWeWishYouLength,
  melodyWhiteChristmasLength,
  melodyJingleBellsLength,
  melodyRudolfTheRedNosedLength,
  melodySantaClausIsCominLength,
  melodySilentNightLength,
  melodyWeThreeKingsLength
};

int melodyTempoList[MELODY_COUNT] = {
  160, 155, 180, 150, 137, 130, 144
};

const int* getMelodyData(int id) {
  if (id >= 0 && id < MELODY_COUNT) return melodyDataList[id];
  return nullptr;
}

int getMelodyLength(int id) {
  if (id >= 0 && id < MELODY_COUNT) return melodyLengthList[id];
  return 0;
}

int getMelodyTempo(int id) {
  if (id >= 0 && id < MELODY_COUNT) return melodyTempoList[id];
  return 120;  // default tempo
}

