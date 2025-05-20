#ifndef MELODY_ENGINE_H
#define MELODY_ENGINE_H

#include <Arduino.h>

void startMelodyPreview(const int* melody, int length, int tempo, int pin);
void updateMelodyPlayback();
bool isMelodyPlaying();
void stopMelody();
const int* getMelodyData(int id);
int getMelodyLength(int id);
int getMelodyTempo(int id);

#endif