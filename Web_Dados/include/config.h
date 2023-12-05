#include <Arduino.h>
#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include "SPIFFS.h"

// Replace with your network credentials
const char* ssid = "WifiEsp";
const char* password = "88599380";

// Set LED GPIO
const int ledPin = 27;
// Stores LED state
String ledState;

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

// Replaces placeholder with LED state value

String processor(const String &var)
{
  // Serial.println(var);
  if (var == "TMC")
  {
     return String();
  }
  else if (var == "TMF")
  {
     return String();
  }
  else if (var == "BOMBA")
  {
     return String();
  }

  else if (var == "HMDT")
  {
     return String();
  }
  else if (var == "SERVO")
  {
     return String();
  }
  else if (var == "SOLO")
  {
     return String();
  }

  return String();
}
 
void initWebServer(){
  // Serial port for debugging purposes
  pinMode(ledPin, OUTPUT);

  // Initialize SPIFFS
  if(!SPIFFS.begin(true)){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP32 Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/index.html", String(), false);
  });
  
  // Route to load style.css file
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });
   server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/script.js","text/javascript");
  });


  // Start server
  server.begin();
}
 
