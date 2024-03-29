#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>
#include "temperatura.h"
#include "humidade.h"
#include "bomba.h"
#include "servo.h"
#include "SPIFFS.h"
#include <Arduino_JSON.h>

// INFO dos button
String MODO_ATUAL = " ";
String BOMBA_STATUS = "OFF";

const char *PARAM_MODE = "state";
const char *PARAM_SERVO = "value";
const char *PARAM_BOMBA = "relay";

AsyncWebServer server(80);

const char *ssid = "WifiEsp";
const char *password = "88599380";

// const char *password = "12345678";
// const char *ssid = "Selma Maria_Fibra";

// CONFIG DO WEB SERVER

// IPAddress local_IP(192, 168, 100, 30);
// IPAddress gateway(192, 168, 100, 1);
// IPAddress subnet(255, 255, 255, 0);

JSONVar readings;

// FUNCOES DO WIFI

String processor(const String &var)
{
  // Serial.println(var);
  if (var == "TMC")
  {
    return temperatureC;
  }
  else if (var == "TMF")
  {
    return temperatureF;
  }
  else if (var == "BOMBA")
  {
    return estado_bomba;
  }

  else if (var == "HMDT")
  {
    return humidity;
  }
  else if (var == "SERVO")
  {
    return estado_servo;
  }
  else if (var == "SOLO")
  {
    return solo;
  }

  return String();
}

void initWifi()
{
  if (!SPIFFS.begin())
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else
  {
    Serial.println("SPIFFS mounted successfully");
  }

  // CONFIG DE IP
  // IPAddress local_IP(192, 168, 141, 31);
  // IPAddress gateway(192, 168, 141, 0);
  // IPAddress subnet(255, 255, 255, 0);
  // WiFi.mode(WIFI_STA);

  /*if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
  }*/

  // Connect to Wi-Fi

  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", String(), false); });

  // Route to load style.css and script.js file

  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/style.css", "text/css"); });
  server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/script.js", "text/javascript"); });

  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDSTemperatureC().c_str()); });
  server.on("/umidade", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDSHumidity().c_str()); });
  server.on("/servo", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", estado_servo.c_str()); });
  server.on("/solo", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", solo.c_str()); });
  server.on("/temperaturef", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", readDSTemperatureF().c_str()); });
  server.on("/butt_bomba", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", estado_bomba.c_str()); });

  server.on("/update", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                String inputMessage;
                String inputParam;

                // GET input1 value on <ESP_IP>/update?state=<inputMessage>
                if (request->hasParam(PARAM_MODE))
                {
                  inputMessage = request->getParam(PARAM_MODE)->value();
                  inputParam = PARAM_MODE;
                }
                else
                {
                  inputMessage = "No message sent";
                  inputParam = "none";
                }
                MODO_ATUAL = inputMessage;
                if ((MODO_ATUAL == " ") || (MODO_ATUAL == "0"))
                {
                   Serial.println("-----------MODO AUTOMÁTICO ----------");
                }
                else if (MODO_ATUAL == "1")
                {
                 Serial.println("-----------MODO MANUAL ----------");
                }
                else
                {
                  Serial.println("-----------ERRO DE MODO ----------");
                }


                request->send(200,"text/plain", "OK"); });

  // SERVO
  server.on("/slider", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      String message;
      String inputParam;

      if (request->hasParam(PARAM_SERVO)) {

        message = request->getParam(PARAM_SERVO)->value();
        inputParam = PARAM_SERVO;
        Serial.println("Position Servo:"+message);
        myservo.write(message.toInt());
        estado_servo = writeSERVO(1);
      }

      else {
        message = "No message sent";
        estado_servo = writeSERVO(0);
      }


      request->send(200,"text/plain", "OK"); });

  // BOMBA DAGUA

  server.on("/bomba", HTTP_GET, [](AsyncWebServerRequest *request)
            {
      String inputParam;

      if (request->hasParam(PARAM_BOMBA)) {

        BOMBA_STATUS = request->getParam(PARAM_BOMBA)->value();
        inputParam = PARAM_BOMBA;
      }
      else {
        BOMBA_STATUS = "No message sent";
        inputParam = "none";
      }
      Serial.println("Status da Bomba:"+BOMBA_STATUS);
      Bomba_Agua(BOMBA_STATUS);
      request->send(200, "text/plain", "OK"); });
 
  // Start server

  server.begin();
}