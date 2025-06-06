// #include "globals.h"
// #include <WiFi.h>
// #include <WiFiClientSecure.h>
// #include "web_server.h"
// #include "secrets.h"
// #include "config.h"


// // External Firebase objects
// WiFiClientSecure ssl_client;
// FirebaseApp app;
// RealtimeDatabase Database;

// // External result processor
// AsyncClientClass aClient(ssl_client);
// //AsyncResult databaseResult;
// UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASS, 4400U);

// bool taskComplete = false;

// // For LED ON/OFF state tracking
// bool lastLedState = false;

// // For LED brightness tracking (start with -1 to force initial write)
// int lastBrightness = -1;

// // For message display tracking
// String lastMessage = "";
// bool ledRequestPending = false;
// bool brightnessRequestPending = false;
// bool messageRequestPending = false;
// unsigned long lastDataCheck = 0;



// void processData(AsyncResult &aResult) {
//   Serial.println("📥 processData called");

//   const String &uid = aResult.uid();

//   // 🔴 Error handling
//   if (aResult.isError()) {
//     Serial.printf("❌ [%s] %s (code %d)\n", 
//                   uid.c_str(), 
//                   aResult.error().message().c_str(), 
//                   aResult.error().code());
//     return;
//   }

//   // 🟡 Debug or server events
//   if (aResult.isDebug()) {
//     Serial.printf("🐞 Debug [%s]: %s\n", uid.c_str(), aResult.debug().c_str());
//   }

//   if (aResult.isEvent()) {
//     Serial.printf("📩 Event [%s]: %s\n", uid.c_str(), aResult.eventLog().message().c_str());
//   }

//   // ✅ Result handling
//   if (aResult.available()) {
//     Serial.printf("✅ Result [%s]: %s\n", uid.c_str(), aResult.c_str());

//     // Reset pending flags (optional if using the flag method)
//     if (uid == "get_led_on")        ledRequestPending = false;
//     else if (uid == "get_led_brightness") brightnessRequestPending = false;
//     else if (uid == "get_message")  messageRequestPending = false;

//     // 🔘 Process LED ON/OFF state
//     if (uid == "get_led_on") {
//       bool newState = strcmp(aResult.c_str(), "true") == 0;
//       if (newState != lastLedState) {
//         lastLedState = newState;
//         if (newState) {
//           // LED ON → use last known brightness
//           ledcWrite(PWM_CHANNEL, lastBrightness);
//         } else {
//           // LED OFF → force PWM to 0
//           ledcWrite(PWM_CHANNEL, 0);
//         }
//       }
//     }

//     // 💡 Process brightness
//     else if (uid == "get_led_brightness") {
//       float brightnessRatio = atof(aResult.c_str());
//       brightnessRatio = constrain(brightnessRatio, 0.0, 1.0);

//       int brightness = round(brightnessRatio * 255);
//       if (brightness != lastBrightness) {
//         lastBrightness = brightness;
//         Serial.printf("💡 Brightness set to %.2f (%d/255)\n", brightnessRatio, brightness);
        
//         // ✅ Only set PWM if LED is ON
//         if (lastLedState) {
//           ledcWrite(PWM_CHANNEL, brightness);
//           Serial.println("if last state changed, new brightness is ");
//           Serial.println(brightness);
//         }
//       }
//     }


//     // 📨 Display new message
//     else if (uid == "get_message") {
//       String newMsg = String(aResult.c_str());
//       if (newMsg != lastMessage) {
//         lastMessage = newMsg;
//         Serial.printf("🪧 New message: %s\n", newMsg.c_str());
//         uiState = MESSAGE_DISPLAY;
//       }
//     }
//   }
// }

// //
// void initFirebase() {
//   // 🔒 Disable SSL cert validation for Firebase (for testing only)
//   ssl_client.setInsecure();

//   // 🔐 Initialize Firebase authentication and background app
//   initializeApp(aClient, app, getAuth(user_auth), processData, "authTask");

//   // 🔗 Attach Firebase RealtimeDatabase app
//   app.getApp<RealtimeDatabase>(Database);
//   Database.url(DATABASE_URL);  // Set the Firebase Realtime DB URL
// }

// bool waitForFirebaseReady(unsigned long timeoutMs) {
//   unsigned long start = millis();
//   while (!app.ready()) {
//     app.loop();
//     if (millis() - start > timeoutMs) {
//       Serial.println("❌ Firebase not ready in time.");
//       return false;
//     }
//     delay(100);
//   }
//   Serial.println("✅ Firebase is ready!");
//   return true;
// }

// void getDataFromFirebase(){
//   // Keep Firebase client running
//   app.loop();

//   // Only perform Firebase GETs when app is ready
//   //Serial.println("enterying Database fetching...");
//   if (app.ready() && millis() - lastDataCheck >= CHECK_INTERVAL) {
//     lastDataCheck = millis();
//     //Serial.println("Firebase ready!");
//     // Get LED ON/OFF state
//     if (!ledRequestPending) {
//       ledRequestPending = true;
//       Database.get(aClient, "/led/on", processData, false, "get_led_on");
//     }

//     if (!brightnessRequestPending) {
//       brightnessRequestPending = true;
//       Database.get(aClient, "/led/brightness", processData, false, "get_led_brightness");
//     }

//     if (!messageRequestPending) {
//       messageRequestPending = true;
//       Database.get(aClient, "/message", processData, false, "get_message");
//     }
//   }
// }


