
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include "FS.h"
#include "SD.h"
#include "SPI.h"
#include "SPIFFS.h"
// Replace with your network credentials

const char *ssid = "Selma Maria_Fibra";
const char *password = "88599380";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

void initSPIFFS()
{
    if (!SPIFFS.begin(true))
    {
        Serial.println("An Error has occurred while mounting SPIFFS");
        return;
    }

    // File file = SPIFFS.open("/index.html", "r");

    // file.close();
}
void initSDCard()
{
    if (!SD.begin())
    {
        Serial.println("Card Mount Failed");
        return;
    }
    uint8_t cardType = SD.cardType();

    if (cardType == CARD_NONE)
    {
        Serial.println("No SD card attached");
        return;
    }

    Serial.print("SD Card Type: ");
    if (cardType == CARD_MMC)
    {
        Serial.println("MMC");
    }
    else if (cardType == CARD_SD)
    {
        Serial.println("SDSC");
    }
    else if (cardType == CARD_SDHC)
    {
        Serial.println("SDHC");
    }
    else
    {
        Serial.println("UNKNOWN");
    }
    uint64_t cardSize = SD.cardSize() / (1024 * 1024);
    Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void initWiFi()
{
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.print("Connecting to WiFi ..");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print('.');
        delay(1000);
    }
    Serial.println(WiFi.localIP());
}

void setup()
{
    Serial.begin(115200);
    initWiFi();
    initSDCard();
    initSPIFFS();

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/index.html", "text/html"); });
    // Route to load style.css file
    server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/style.css", "text/css"); });
    // Route to load scripts.js file
    server.on("/script.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/script.js", "text/js"); });

    server.on("/components/highcharts.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/components/highcharts.js", "javascript"); });
    server.on("/components/jquery.min.js", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/components/jquery.min.js", "javascript"); });
    server.on("/img/header.avif", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/img/header.avif", "img"); });
    server.on("/icon/github.svg", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/img/github.svg", "icon"); });
    server.on("/icon/linkedin.svg", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send(SD, "/img/linkedin.svg", "icon"); });
    server.begin();
}

void loop()
{
    delay(20);
}