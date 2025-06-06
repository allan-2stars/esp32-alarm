#pragma once
#include "alarm.h"

class AlarmPlayerService {
public:
  // void playAlarm(const Alarm& alarm, bool loop);//, bool withLights);
  void playAlarm(const Alarm& alarm, bool loop = true, bool withLights = true);

  void stopAlarm();
  bool isAlarmPlaying() const;

private:
  bool playing = false;
};
