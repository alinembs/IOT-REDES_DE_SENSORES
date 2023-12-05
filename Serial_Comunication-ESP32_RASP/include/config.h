#include <WiFi.h>

#include <WiFiClient.h>

#include <ArduinoJson.h>

#include "SPIFFS.h"

#include "FS.h"

#include <WebServer.h>

#include <HTTPClient.h>

const char *apiEndpoint_2 = "http://192.168.29.139:5000/bomba";
const char *apiEndpoint_1 = "http://192.168.29.139:5000/braco";
const char *apiKey = "API_TESTE";
const char *PARAM_BOMBA = "relay";
const char *PARAM_SERVO = "value";
int POSITION_SERVO = 0;
String valor = "";
WebServer server(80);

void handleNotFound()
{
    server.send(404, "text/plain", "Página não encontrada");
}


// MANIPULADOPR PARA CONTROLAR O BRAÇO ROBOTICO - BASE
void handle_controle_braco1()
{

  String message = "";

  if (server.arg(PARAM_SERVO) == "")
  { // Parameter not found

    message = "Argument not found";
  }
  else
  { // Parameter found

    message += server.arg(PARAM_SERVO); // Gets the value of the query parameter
    POSITION_SERVO = message.toInt();
  }

  DynamicJsonDocument jsonDoc(256);
  jsonDoc["index_servo"] = "Base";
  jsonDoc["position_servo"] = POSITION_SERVO;
  String requestBody;
  serializeJson(jsonDoc, requestBody);

  HTTPClient http;
  http.begin(apiEndpoint_1);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Api-Key", apiKey);

  int httpResponseCode = http.POST(requestBody);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.println(response);
    server.send(200, "text/html", "<h1>Dados enviados com sucesso para a API!</h1>");
  }
  else
  {
    Serial.printf("Erro na solicitação HTTP: %s\n", http.errorToString(httpResponseCode).c_str());
    server.send(500, "text/html", "<h1>Erro na solicitação HTTP</h1>");
  }

  http.end();
}

// MANIPULADOPR PARA CONTROLAR O BRAÇO ROBOTICO - HORIZONTAL
void handle_controle_braco2()
{

  String message = "";

  if (server.arg(PARAM_SERVO) == "")
  { // Parameter not found

    message = "Argument not found";
  }
  else
  { // Parameter found

    message += server.arg(PARAM_SERVO); // Gets the value of the query parameter
    POSITION_SERVO = message.toInt();
  }

  DynamicJsonDocument jsonDoc(256);
  jsonDoc["index_servo"] = "Horizontal";
  jsonDoc["position_servo"] = POSITION_SERVO;
  String requestBody;
  serializeJson(jsonDoc, requestBody);

  HTTPClient http;
  http.begin(apiEndpoint_1);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Api-Key", apiKey);

  int httpResponseCode = http.POST(requestBody);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.println(response);
    server.send(200, "text/html", "<h1>Dados enviados com sucesso para a API!</h1>");
  }
  else
  {
    Serial.printf("Erro na solicitação HTTP: %s\n", http.errorToString(httpResponseCode).c_str());
    server.send(500, "text/html", "<h1>Erro na solicitação HTTP</h1>");
  }

  http.end();
}
// MANIPULADOPR PARA CONTROLAR O BRAÇO ROBOTICO - VERTICAL
void handle_controle_braco3()
{

  String message = "";

  if (server.arg(PARAM_SERVO) == "")
  { // Parameter not found

    message = "Argument not found";
  }
  else
  { // Parameter found

    message += server.arg(PARAM_SERVO); // Gets the value of the query parameter
    POSITION_SERVO = message.toInt();
  }

  DynamicJsonDocument jsonDoc(256);
  jsonDoc["index_servo"] = "Vertical";
  jsonDoc["position_servo"] = POSITION_SERVO;
  String requestBody;
  serializeJson(jsonDoc, requestBody);

  HTTPClient http;
  http.begin(apiEndpoint_1);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("X-Api-Key", apiKey);

  int httpResponseCode = http.POST(requestBody);

  if (httpResponseCode > 0)
  {
    String response = http.getString();
    Serial.println(response);
    server.send(200, "text/html", "<h1>Dados enviados com sucesso para a API!</h1>");
  }
  else
  {
    Serial.printf("Erro na solicitação HTTP: %s\n", http.errorToString(httpResponseCode).c_str());
    server.send(500, "text/html", "<h1>Erro na solicitação HTTP</h1>");
  }

  http.end();
}

// MANIPULADOPR PARA CONTROLAR A BOMBA DE AGUA
void handle_controle_bomba()
{

    DynamicJsonDocument jsonDoc(256);
    jsonDoc["state_bomba"] = valor;
    String requestBody;
    serializeJson(jsonDoc, requestBody);

    HTTPClient http;
    http.begin(apiEndpoint_2);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", apiKey);

    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0)
    {
        String response = http.getString();
        Serial.println(response);
        server.send(200, "text/html", "<h1>Solicitação concluida com sucesso na API!</h1>");
    }
    else
    {
        Serial.printf("Erro na solicitação HTTP: %s\n", http.errorToString(httpResponseCode).c_str());
        server.send(500, "text/html", "<h1>Erro na solicitação HTTP</h1>");
    }

    http.end();
}

void initSPIFFS()
{
    if (!SPIFFS.begin())
    {
        Serial.println("An error has occurred while mounting SPIFFS");
    }
    else
    {
        Serial.println("SPIFFS mounted successfully");
    }

    // File file = SPIFFS.open("/index.html", "r");

    // file.close();
}
// Pegar dados do SPIFFS
bool loadFromSPIFFS(String path, String dataType)
{
    Serial.print("Requested page -> ");
    Serial.println(path);
    if (SPIFFS.exists(path))
    {
        File dataFile = SPIFFS.open(path, "r");
        if (!dataFile)
        {
            handleNotFound();
            return false;
        }

        if (server.streamFile(dataFile, dataType) != dataFile.size())
        {
            Serial.println("Sent less data than expected!");
        }
        else
        {
            Serial.println("Page served!");
        }

        dataFile.close();
    }
    else
    {
        handleNotFound();
        return false;
    }
    return true;
}

// Manipulador para a página principal
void handleRoot()
{
    // loadFromSD(SD, "/index.html", "text/html");
    loadFromSPIFFS("/home.html", "text/html");
}

void handleLEDArg()
{

    String message = "";

    if (server.arg(PARAM_BOMBA) == "")
    { // Parameter not found

        message = "Argument not found";
    }
    else
    { // Parameter found

        message += server.arg(PARAM_BOMBA); // Gets the value of the query parameter
    }

    valor = message;
    handle_controle_bomba();
    server.send(200, "text / plain", "OK"); // Returns the HTTP response
}

const char *ssid = "ESP-DASHBOARD";
const char *password = "123456789";

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// Iniciar Wifi no Modo Acess Point
void init_Wifi_AP()
{
    Serial.println("Configuring access point...");

    if (!WiFi.softAPConfig(local_ip, gateway, subnet))
    {
        Serial.println("STA Failed to configure");
    }
    delay(1);
    if (!WiFi.softAP(ssid, password))
    {
        log_e("Soft AP creation failed.");
        while (1)
            ;
    }

    IPAddress myIP = WiFi.softAPIP();
    Serial.print("AP IP address: ");
    Serial.println(myIP);
}
// Iniciar Wifi no Modo Normal
void init_Wifi_NM()
{
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Conectando ao WiFi...");
    }
    Serial.println("Conectado ao WiFi!");
    IPAddress myIP = WiFi.localIP();
    Serial.print("IP address: ");
    Serial.println(myIP);
}

void init_Server()
{

    // Adiciona a função "handle_on_connect" quando o servidor estiver online
    server.on("/", HTTP_GET, handleRoot);
    // Rota para o Braço Robotico
    //Base
    server.on("/slider1", HTTP_GET, handle_controle_braco1);
    //Horizontal
    server.on("/slider2", HTTP_GET, handle_controle_braco2);
    //Vertical
    server.on("/slider3", HTTP_GET,handle_controle_braco3);
    // Rota para ligar a Bomba
    server.on("/update", HTTP_GET, handleLEDArg);
    //Rota caso 
    server.onNotFound(handleNotFound);

    // Inicia o servidor
    server.begin();

    Serial.println("Server started");
}
