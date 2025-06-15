#pragma once

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SCREEN_CONTRAST 255

#define OLED_RESET -1
#define SDA_PIN 16
#define SCL_PIN 17

//
// ####### Config For Wokwi / Real ESP32 #######
//
#define RESET_BUTTON_PIN 14   // Real ESP32
// #define RESET_BUTTON_PIN 32  // Wokwi
//#define DHTPIN 14   //Wokwi
#define DHTTYPE DHT22   //Wokwi
#define BUZZER_PIN  2
#define RED_PIN    18   // Wokwi
#define GREEN_PIN  19   // Wokwi
#define BLUE_PIN   23   
/// ###############################

// Sensors
#define TOUCH_WAKE_PIN T3 // GPIO15 integrated
//#define SHOCK_SENSOR_PIN 4
#define SHOCK_SENSOR_PIN 0 // Wokwi
#define SERVO_PIN 13 // for open the door

#define SCREEN_CONTRAST_PIN 21
#define MODE_BUTTON_PIN     33
// #define ADJUST_BUTTON_PIN   27
// #define CONFIRM_BUTTON_PIN  34
#define ADJUST_BUTTON_PIN   5 // Wokwi
#define CONFIRM_BUTTON_PIN  4 // Wokwi

#define UI_TIMEOUT_MS  30000  // 30 seconds back to Idle screen
#define CHECK_INTERVAL 6000 //check connection timeout
#define HEADER_HEIGHT 16  // vertical space reserved for top icons
#define TEXT_COLOR SSD1306_WHITE

#define MAX_SCREEN_ALARMS 3
#define MAX_TOTAL_ALARMS 10
#define SCREEN_ALARM_VERSION 0xA1A1
#define WEB_ALARM_VERSION    0xB2B2

// this is the 3 brightest LED on top
#define LED_LEFT_PIN 32
#define LED_TOP_PIN 22
#define LED_RIGHT_PIN 25

#define LED_PIN 12
#define PWM_CHANNEL 6
#define PWM_FREQ 5000
#define PWM_RESOLUTION 8

#define CHANNEL_RED    0
#define CHANNEL_GREEN  1
#define CHANNEL_BLUE   2

#define INACTIVITY_TIMEOUT 600000 // 1 min then sleep

#define CHANNEL_LEFT_LED   3
#define CHANNEL_TOP_LED  4
#define CHANNEL_RIGHT_LED  5

//#### Alarm light stage durations (in milliseconds)
#define ALARM_STAGE1_DURATION  10000
#define ALARM_STAGE2_DURATION  20000
#define ALARM_STAGE3_DURATION  30000
#define ALARM_STAGE4_DURATION  40000

// Fade timing
#define FADE_PERIOD_NORMAL  3000
#define FADE_PERIOD_FAST    1000

// Flashing
#define FLASH_TOGGLE_INTERVAL 500

// Max LED brightness
#define LED_MAX_BRIGHTNESS 255
//###################################

#define ENABLE_USER_AUTH
#define ENABLE_DATABASE
// #include <FirebaseClient.h>

// in case you need them below for Firebase
// ENABLE_DATABASE // For RTDB compilation
// ENABLE_FIRESTORE // For Firestore compilation
// ENABLE_FIRESTORE_QUERY // For Firestore Query feature compilation
// ENABLE_MESSAGING // For Firebase Cloud Messaging compilation
// ENABLE_STORAGE // For Firebase Storage compilation
// ENABLE_CLOUD_STORAGE // For Google Cloud Storage compilation
// ENABLE_FUNCTIONS // For Google Cloud Functions compilation
// ENABLE_RULESETS // For RuleSets compilation
// ENABLE_PSRAM // For enabling PSRAM support
// ENABLE_OTA // For enabling OTA updates support
// ENABLE_FS // For enabling Flash filesystem support

// FIREBASE_ASYNC_QUEUE_LIMIT // For maximum async queue limit (number).
// FIREBASE_PRINTF_PORT // For Firebase.printf debug port class object.
// FIREBASE_PRINTF_BUFFER // Firebase.printf buffer size.

// // For enabling authentication and token
// ENABLE_SERVICE_AUTH
// ENABLE_CUSTOM_AUTH
// ENABLE_USER_AUTH
// ENABLE_ACCESS_TOKEN
// ENABLE_CUSTOM_TOKEN
// ENABLE_ID_TOKEN
// ENABLE_LEGACY_TOKEN
