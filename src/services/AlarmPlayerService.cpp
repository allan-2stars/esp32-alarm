
#include "services/MelodyService.h"
#include "services/LedService.h"
#include "services/AlarmPlayerService.h"
#include "globals.h"
#include "config.h"
#include "melodies.h"

void AlarmPlayerService::playAlarm(const Alarm& alarm, bool loop, bool withLights) {
  stopAlarm();  // Always stop before starting new

  melodyService.play(
    getMelodyData(alarm.melody),
    getMelodyLength(alarm.melody),
    getMelodyTempo(alarm.melody),
    BUZZER_PIN,
    loop  // ✅ dynamic based on caller
  );

  if (withLights) ledService.startAlarmLights();
  playing = true;
}


void AlarmPlayerService::stopAlarm() {
  melodyService.stop();
  ledService.stopAlarmLights();
  playing = false;

}

bool AlarmPlayerService::isAlarmPlaying() const {
  return playing;
}
