
#include "led.h"
#include "config.h"

static unsigned long lastToggleTime = 0;
static bool ledOn = false;
static unsigned long blinkInterval = 500;
static uint8_t blinkR = 0, blinkG = 0, blinkB = 0;
static LedMode currentMode = LED_OFF;

void setLedMode(LedMode mode) {
  if (mode != currentMode) {
    currentMode = mode;
    switch(mode){
    case LED_OFF:setLedColor(0, 0, 0);break;
    case LED_WIFI:blinkLed(0, 255, 0, 300);break;  // Green
    case LED_MELODY:blinkLed(220, 100, 3, 250);break; // 
    case LED_CUSTOM:break;// LED_CUSTOM: user can control manually
    }
  }
}

void setupRGBLed() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, LOW);
  digitalWrite(BLUE_PIN, LOW);
}

void setLedColor(uint8_t r, uint8_t g, uint8_t b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void blinkLed(uint8_t r, uint8_t g, uint8_t b, unsigned long intervalMs) {
  blinkR = r;
  blinkG = g;
  blinkB = b;
  blinkInterval = intervalMs;
  lastToggleTime = millis();
  ledOn = false;
}

void updateLed() {
  unsigned long now = millis();
  if (now - lastToggleTime >= blinkInterval) {
    lastToggleTime = now;
    ledOn = !ledOn;
    if (ledOn) {
      setLedColor(blinkR, blinkG, blinkB);
    } else {
      setLedColor(0, 0, 0);
    }
  }
}
