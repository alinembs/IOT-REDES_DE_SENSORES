
/*Biblioteca Servo | Bomba | Temperatura | Umidade | Sensor de Fluxo */

#include "config_braco.h"
#include "config_bomba.h"
#include "config_humidade.h"
#include "config_temperatura.h"

#include <WebServer.h>
#include <ESPmDNS.h>

#include <WiFi.h>
#include <WiFiClient.h>

#include <SPI.h>

const char *ssid = "ESPWIFI";
const char *password = "1234567890";

const char *PARAM_MODE = "state";
const char *PARAM_FILE = "value";

const char *PARAM_BOMBA = "status";

const char *PARAM_BASE = "value_base";
const char *PARAM_HORI = "value_hori";
const char *PARAM_VERT = "value_vert";

const int maxTentativas = 5;
int tentativas = 0;

WebServer server(80);

#define servername "MWSN" // Define the name to server...

// /* Put IP Address details */
// IPAddress local_IP(192, 168, 1, 2);
//  IPAddress gateway(192, 168, 1, 1);
//  IPAddress subnet(255, 255, 255, 0);

// Iniciar Wifi no Modo Acess Point
void init_Wifi_AP()
{
  Serial.println("Configuring access point...");
  delay(1);
  if (!WiFi.softAP(ssid, password))
  {
    log_e("Soft AP creation failed.");
    while (1)
      ;
  }
  // Set your preferred server name, if you use "plantavision" the address would be http://MWSN.local/
  if (!MDNS.begin(servername))
  {
    Serial.println(F("Error setting up MDNS responder!"));
    ESP.restart();
  }

  Serial.println("MDNS started");

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

// Iniciar Wifi no Modo Normal
void init_Wifi_NM()
{
  while (tentativas < maxTentativas)
  {
    WiFi.begin(ssid, password);

    int contagemTempo = 0;
    while (WiFi.status() != WL_CONNECTED && contagemTempo < 20)
    {
      delay(500);
      Serial.println("Conectando ao WiFi...");
      contagemTempo++;
    }
    // Verificar se a conexão foi estabelecida
    if (WiFi.status() == WL_CONNECTED)
    {
      Serial.println("\nConectado ao WiFi!");
      break; // Sair do loop se a conexão for bem-sucedida
    }
    else
    {
      Serial.println("\nFalha na conexão. Tentando novamente...");
      tentativas++;
    }

    // Se as tentativas excederem o limite, reiniciar o ESP32
    if (tentativas >= maxTentativas)
    {
      Serial.println("Número máximo de tentativas atingido. Reiniciando...");
      ESP.restart();
    }
  }

  // Set your preferred server name, if you use "plantavision" the address would be http://plantavision.local/
  if (!MDNS.begin(servername))
  {
    Serial.println(F("Error setting up MDNS responder!"));
    ESP.restart();
  }

  Serial.println("MDNS started");

  IPAddress serverIp;
  Serial.println("Host address resolved:");
  Serial.println(serverIp.toString());
  IPAddress myIP = WiFi.localIP();
  Serial.print("IP address: ");
  Serial.println(myIP);
}

// void init_Wifi_NM1()
// {
//   if (!WiFi.config(local_IP, gateway, subnet)) {
//     Serial.println("STA Failed to configure");
//   }

//   WiFi.begin(ssid, password);
//   while (WiFi.status() != WL_CONNECTED) {
//     delay(1000);
//     Serial.println("Conectando ao WiFi...");
//   }
//   Serial.println("Conectado ao WiFi!");
//   IPAddress myIP = WiFi.localIP();
//     Serial.print("IP address: ");
//     Serial.println(myIP);

// }

void handleNotFound()
{
  server.send(404, "text/plain", "Página não encontrada");
}
// Manipulador para a página principal
void handleRoot()
{
  server.send(200, "text/plain", "Planta Vision - BACK_END");
}

void handleGenericArgs()
{ // Handler

  String message = "Number of args received:";
  message += server.args(); // Get number of parameters
  message += "\n";          // Add a new line

  for (int i = 0; i < server.args(); i++)
  {

    message += "Arg nº" + (String)i + " –> "; // Include the current iteration value
    message += server.argName(i) + ": ";      // Get the name of the parameter
    message += server.arg(i) + "\n";          // Get the value of the parameter
  }

  server.send(200, "text/plain", message); // Response to the HTTP request
}

void handleServoBASE()
{

  String message = "";

  if (server.arg(PARAM_BASE) == "")
  { // Parameter not found

    message = "Argument not found";
  }
  else
  { // Parameter found

    message += server.arg(PARAM_BASE); // Gets the value of the query parameter
    int valor = message.toInt();
    servo_base.write(valor);
    Serial.println("Posicao_BASE");
    Serial.println(valor);
  }

  server.send(200, "text / plain", "OK"); // Returns the HTTP response
}
void handleServoHORI()
{

  String message = "";

  if (server.arg(PARAM_HORI) == "")
  { // Parameter not found

    message = "Argument not found";
  }
  else
  { // Parameter found

    message += server.arg(PARAM_HORI); // Gets the value of the query parameter
    int valor = message.toInt();
    servo_horizontal.write(valor);
    Serial.println("Posicao_HORI");
    Serial.println(valor);
  }

  server.send(200, "text / plain", "OK"); // Returns the HTTP response
}
void handleServoVERT()
{

  String message = "";

  if (server.arg(PARAM_VERT) == "")
  { // Parameter not found

    message = "Argument not found";
  }
  else
  { // Parameter found

    message += server.arg(PARAM_VERT); // Gets the value of the query parameter
    int valor = message.toInt();
    servo_vertical.write(valor);
    Serial.println("Posicao_VERT");
    Serial.println(valor);
  }

  server.send(200, "text / plain", "OK"); // Returns the HTTP response
}

void handleBombArg()
{

  String message = "";

  if (server.arg(PARAM_BOMBA) == "")
  { // Parameter not found

    message = "Argument not found";
  }
  else
  { // Parameter found

    message += server.arg(PARAM_BOMBA); // Gets the value of the query parameter
    Controle_Bomba(message);
  }

  // Bomba_Agua(message);
  server.send(200, "text / plain", "OK"); // Returns the HTTP response
}
void sensorsdata()
{

  String json = "{\"temperatura_c\":";
  json += readDSTemperatureC();
  // json += "}";
  json += ",";
  json += "\"temperatura_f\":";
  json += readDSTemperatureF();
  // json += "}";
    json += ",";
  json += "\"umidade\":";
  json += onSensorChange();
  // json += "}";
  json += ",";
  json += "\"flowrate\":";
  json += flowrate_state;
  // json += "}";
  json += ",";
  json += "\"totalwater\":";
  json += total_water_state;
  json += "}";

  server.send(200, "application/json", json);
}
void json_Tempc()
{
  String json = "{\"temperatura_c\":";
  json += readDSTemperatureC();
  json += "}";
  server.send(200, "application/json", json);
}
void json_Tempf()
{
  String json = "{\"temperatura_f\":";
  json += readDSTemperatureF();
  json += "}";
  server.send(200, "application/json", json);
}
void json_umidade()
{
  String json = "{\"umidade\":";
  json += onSensorChange();
  json += "}";
  server.send(200, "application/json", json);
}

void flowrate_cv()
{
  String json = "{\"flowrate\":";
  json += flowrate_state;
  json += "}";
  server.send(200, "application/json", json);
}
void total_water()
{
  String json = "{\"totalwater\":";
  json += total_water_state;
  json += "}";
  server.send(200, "application/json", json);
}

void handleSN()
{
  server.send(200, "text / plain", "OK"); // Returns the HTTP response
}
// Inicializa o Servidor para controla o braço e a bomba
void init_Server()
{
  // Configuração das Rotas do Servidor
  // Adiciona a função "handle_on_connect" quando o servidor estiver online
  server.on("/", HTTP_GET, handleRoot);

  server.on("/autenticar", HTTP_GET, handleRoot);

  // Rota  do Controle de FLuxo
  server.on("/flowrate", HTTP_GET, flowrate_cv);
  server.on("/total", HTTP_GET, total_water);

  // Rota dos Sensores
  server.on("/temperature_f", HTTP_GET, json_Tempf);
  server.on("/temperature_c", HTTP_GET, json_Tempc);
  server.on("/umidade", HTTP_GET, json_umidade);

  // Funções para Mandar o RaspiBerry Pi controlor o Braco e ou Bomba

  server.on("/servo_base", HTTP_GET, handleServoBASE);
  server.on("/servo_hori", HTTP_GET, handleServoHORI);
  server.on("/servo_vert", HTTP_GET, handleServoVERT);
  server.on("/bomba", HTTP_GET, handleBombArg);

  //Rota para Enviar os dados de todos os sensores:
  server.on("/sensor_node_data",HTTP_GET,sensorsdata);

  //  Adiciona a função "handle_not_found" quando o servidor estiver offline
  server.onNotFound(handleNotFound);
  // Inicia o servidor
  server.begin();

  Serial.println("Server MWSN started");
}
