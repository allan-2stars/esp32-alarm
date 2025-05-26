#include <Adafruit_AHTX0.h>
#include "config.h"

Adafruit_AHTX0 aht10;
sensors_event_t humidity, temp;

bool isAHTInitialized = false;

void initAHT10() {
  if (!aht10.begin()) {
    Serial.println("❌ Failed to initialize AHT10 sensor!");
    isAHTInitialized = false;
  } else {
    //Serial.println("✅ AHT10 sensor initialized.");
    isAHTInitialized = true;
  }
}

void initBuzzer(){
    digitalWrite(BUZZER_PIN, LOW);
}

float getTemperature() {
  if (!isAHTInitialized) return NAN;

  aht10.getEvent(&humidity, &temp);
  return temp.temperature;
}

float getHumidity() {
  if (!isAHTInitialized) return NAN;

  aht10.getEvent(&humidity, &temp);
  return humidity.relative_humidity;
}
