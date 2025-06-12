#pragma once

class SleepManager {
public:
  void begin();
  void update();  // Call this from loop()

private:
  void enterSleep();
  bool isNearAlarm();
};
