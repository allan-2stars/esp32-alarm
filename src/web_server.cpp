#include <ESPAsyncWebServer.h>
#include <LittleFS.h>
#include "web_server.h"

AsyncWebServer server(80);

void startWebServer() {
  if (!LittleFS.begin()) {
    Serial.println("Failed to mount LittleFS");
    return;
  }

  server.serveStatic("/", LittleFS, "/").setDefaultFile("index.html");

  server.onNotFound([](AsyncWebServerRequest *request){
    request->send(404, "text/plain", "Not Found");
  });

  server.begin();
  Serial.println("Web server started.");
}