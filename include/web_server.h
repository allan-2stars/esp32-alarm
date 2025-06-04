#pragma once

// Main entry point to initialize Firebase, NTP, and PWM
void initFirebase();
bool waitForFirebaseReady(unsigned long timeoutMs);
// Callback to handle Firebase async results
void processData(AsyncResult &aResult);
void getDataFromFirebase();
