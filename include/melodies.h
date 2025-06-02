#ifndef MELODIES_H
#define MELODIES_H

//#define MELODY_COUNT 6 // when update the numbers of melody, modify the number.

extern const int melodyWeWishYou[];
extern const int melodyWeWishYouLength;

extern const int melodyWhiteChristmas[];
extern const int melodyWhiteChristmasLength;

extern const int melodyJingleBells[];
extern const int melodyJingleBellsLength;

extern const int melodyRudolfTheRedNosed[];
extern const int melodyRudolfTheRedNosedLength;

extern const int melodySantaClausIsComin[];
extern const int melodySantaClausIsCominLength;

extern const int melodySilentNight[];
extern const int melodySilentNightLength;

extern const char* melodyNames[];
extern const int MELODY_COUNT;

const int* getMelodyData(int id);
int getMelodyLength(int id);
int getMelodyTempo(int id);




#endif
