#pragma once

class MelodyService {
public:
  void begin();
  void play(const int* melody, int length, int tempo, int pin, bool loop);
  void update();
  void stop();
  bool isPlaying() const;

private:
  const int* currentMelody = nullptr;
  int melodyLength = 0;
  int melodyTempo = 0;
  int melodyIndex = 0;
  unsigned long lastNoteTime = 0;
  int buzzerPin = -1;
  bool playing = false;
  bool looping = false;
};