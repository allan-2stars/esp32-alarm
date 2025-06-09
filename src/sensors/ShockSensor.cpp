#include "sensors/ShockSensor.h"
#include "config.h"

void ShockSensor::begin() {
  pinMode(SHOCK_SENSOR_PIN, INPUT);
}

void ShockSensor::update() {
  int state = digitalRead(SHOCK_SENSOR_PIN);

  // Only trigger on clean HIGH and debounce time passed
  if (state == HIGH && millis() - lastShockTime > 100) {
    lastShockTime = millis();
    shocked = true;
  }

  if (shocked && millis() - lastShockTime >= SHOCK_TIMEOUT) {
    shocked = false;
  }
}


bool ShockSensor::isShocked() const {
  return shocked;
}
