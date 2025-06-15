#include "sensors/ServoMotionController.h"
#include "config.h"
#include <time.h>

ServoMotionController::ServoMotionController() {}

void ServoMotionController::begin() {
  ESP32PWM::allocateTimer(0);  // Allocate a dedicated PWM timer
  servo.setPeriodHertz(50);    // Typical servo frequency
  servo.attach(SERVO_PIN, 500, 2400);  // Min/max pulse width (µs)
  pinMode(PIR_PIN, INPUT);
  servo.write(0);  // Initialize position
}

void ServoMotionController::setExcludedHours(const std::vector<int>& hours) {
  excludedHours = hours;
}

void ServoMotionController::update() {
  checkHourlyTrigger();
  checkPIRMotion();
}

bool ServoMotionController::isExcludedHour(int hour) {
  for (int h : excludedHours) {
    if (h == hour) return true;
  }
  return false;
}

void ServoMotionController::checkHourlyTrigger() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) return;

  if (timeinfo.tm_min == 0 && timeinfo.tm_hour != lastHour) {
    lastHour = timeinfo.tm_hour;

    if (!isExcludedHour(timeinfo.tm_hour)) {
      turnServoTo(SERVO_HOURLY_ANGLE);
    }
  }
}

void ServoMotionController::checkPIRMotion() {
  if (digitalRead(PIR_PIN) == HIGH && millis() - lastMotionTime > 2000) {
    Serial.println("moved..");
    turnServoTo(SERVO_MOTION_ANGLE);
    lastMotionTime = millis();
  }
}

void ServoMotionController::turnServoTo(int angle) {
  servo.write(angle);
  Serial.println("turned");
  delay(5000);         // Small hold time
  servo.write(0);     // Return to base position
}
