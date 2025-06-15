#include <ESP32Servo.h>

// Define the servo object
Servo myServo;

// Define the pin connected to the servo signal
const int servoPin = 13; // Use any PWM-capable pin on the ESP32

void setup() {
  // Attach the servo to the specified pin
  myServo.attach(servoPin);

  // Optional: Set the servo's min and max pulse width (in microseconds)
  // Default values are 544 and 2400
  myServo.setPeriodHertz(50); // Standard 50Hz for servos
  myServo.writeMicroseconds(1000); // Set initial position
}

void loop() {
  // Sweep the servo from 0 to 180 degrees
  for (int angle = 0; angle <= 180; angle++) {
    myServo.write(angle); // Move to the specified angle
    delay(15);            // Wait for the servo to reach the position
  }

  // Sweep the servo back from 180 to 0 degrees
  for (int angle = 180; angle >= 0; angle--) {
    myServo.write(angle);
    delay(15);
  }
}
