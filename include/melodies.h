#ifndef MELODIES_H
#define MELODIES_H

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

const int* getMelodyData(int id);
int getMelodyLength(int id);
int getMelodyTempo(int id);

#endif
