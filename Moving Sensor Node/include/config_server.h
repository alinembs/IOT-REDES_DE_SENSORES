
/*Biblioteca Servo | Bomba | Temperatura | Umidade | Sensor de Fluxo | RTC | SD | SPIFSS| WIFI | SERVIDOR */

#include "config_braco.h"
#include "config_bomba.h"
#include "config_humidade.h"
#include "config_temperatura.h"

#include <WebServer.h>
#include <ESPmDNS.h>

#include <WiFi.h>
#include <WiFiClient.h>

#include <SPI.h>
#include "SPIFFS.h"

#include "FS.h"
#include "SD.h"

#include "SPI.h"

#include <Wire.h>
#include <RTClib.h>

#include <Preferences.h>
Preferences preferences;

RTC_DS3231 rtc;
String data_hora = "";

const int ledPin = 2; // Pino do LED interno do ESP32

const char *rede1 = "ESPWIFI";
const char *password1 = "1234567890";

const char *rede2 = "REDE IOT";
const char *password2 = "L@ps1234";

const char *PARAM_MODE = "state";
const char *PARAM_FILE = "value";

const char *PARAM_BOMBA = "status";

const char *PARAM_BASE = "value_base";
const char *PARAM_HORI = "value_hori";
const char *PARAM_VERT = "value_vert";

const char *apiToken = "AGRICULTURE_PRECISION_V2_MONOGRAFIA_UEMA_2024";

const char *username = "LAPS_IOT";
const char *userPassword = "L@ps1234";

const int maxTentativas = 5;
int tentativas = 0;

String Status_RTC = "";
String Status_SD = "";
String Status_SPIFSS = "";

WebServer server(80);

// Iniciar o Cartão de Memoria
void initSDCard()
{
  if (!SD.begin(5))
  {
    Serial.println("Card Mount Failed");
    Status_SD = "FALSE";
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
  Status_SD = "TRUE";
}
// Inicia o SPIFFS
void initSPIFFS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("An error has occurred while mounting SPIFFS");
    Status_SPIFSS = "FALSE";
  }
  else
  {
    Serial.println("SPIFFS mounted successfully");
    Status_SPIFSS = "TRUE";
  }

  // File file = SPIFFS.open("/index.html", "r");

  // file.close();
}
void handleNotFound()
{
  server.send(404, "text/plain", "Página não encontrada");
}
// Pegar arquivos do SD
bool loadFromSD(fs::FS &fs, String path, String dataType)
{
  Serial.print("Requested page -> ");
  Serial.println(path);
  if (fs.exists(path))
  {
    File dataFile = fs.open(path, "r");
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
// Possiblitar baixar arquivos que estão no SD para o Navegador
void SD_file_download(String filename)
{
  File download = SD.open("/" + filename);
  if (download)
  {
    server.sendHeader("Content-Type", "text/text");
    server.sendHeader("Content-Disposition", "attachment; filename=" + filename);
    server.sendHeader("Connection", "close");
    server.streamFile(download, "application/octet-stream");
    download.close();
  }
}
// Escrever dados em um arquivo dentro do SD/SPIFFS
void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("File written");
  }
  else
  {
    Serial.println("Write failed");
  }
  file.close();
}
// Adicionar dados a arquivos já criados dentro do SD/SPIFFS
void appendFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.println(message))
  {
    Serial.println("Message appended");
  }
  else
  {
    Serial.println("Append failed");
  }
  file.close();
}
// Inicializa o RTC e Ajusta a Data-Hora do Esp32 se for a primeira vez
void init_RTC()
{
  preferences.begin("my-app", false);
  Wire.begin();
  if (!rtc.begin())
  {
    Serial.println("Não foi possível encontrar RTC");
    Status_RTC = "FALSE";
    // while(1);
  }
  else
  {
    bool data_modulo = preferences.getBool("data_modulo", false);
    if (data_modulo == false)
    {

      rtc.adjust(DateTime(__DATE__, __TIME__));
      Serial.println("Data e hora definidas!");
      data_modulo = true;
    }
    preferences.putBool("data_modulo", data_modulo);
    preferences.end();
    Status_RTC = "TRUE";
  }
}

// Pega a Data-Hora Atual e salva em uma Variavel Global
String data_now()
{
  DateTime now = rtc.now();
  // Formato da data: DD/MM/AAAA
  String data = String(now.day()) + "/" + String(now.month()) + "/" + String(now.year());
  // Serial.println(data);
  //  Formato da hora: HH:MM:SS
  String hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  // Serial.println(hora);
  return String(data + '~' + hora);
}

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
  if (!WiFi.softAP(rede1, password1))
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
    WiFi.begin(rede1, password1);

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
bool conectarWiFi(const char *ssid, const char *password)
{
  Serial.println("Conectando ao WiFi...");
  WiFi.begin(ssid, password);

  int tentativas = 0;
  while (WiFi.status() != WL_CONNECTED && tentativas < 5)
  {
    delay(1000);
    Serial.print(".");
    tentativas++;
  }
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("\nConexão estabelecida!");
    return true;
  }
  else
  {
    Serial.println("\nFalha ao conectar ao WiFi!");
    return false;
  }
}

// Iniciar Wifi no Modo Normal Versão 2
void init_Wifi_NM2()
{
  // Tenta conectar ao primeiro WiFi
  if (conectarWiFi(rede1, password1))
  {
    Serial.println("Conectado ao WiFi 1");
    digitalWrite(ledPin, HIGH); // Liga o LED
  }
  else
  {
    // Se a conexão ao primeiro WiFi falhar, tenta conectar ao segundo WiFi
    if (conectarWiFi(rede2, password2))
    {
      Serial.println("Conectado ao WiFi 2");
      digitalWrite(ledPin, HIGH); // Liga o LED
    }
    else
    {
      Serial.println("Falha ao conectar ao WiFi. Reiniciando...");
      ESP.restart(); // Reinicia o ESP32
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


// void AtualizaoOTA()
// {
//   loadFromSPIFFS("/mwsn-app/index.html", "text/html");
// }
// Manipulador para a página principal


void handleRoot()
{
  server.send(200, "text/plain", "MWSN - ESP32");
}

void handleGenericArgs()
{ // Handler
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }
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
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }
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
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }
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
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }

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
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }

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
void config_esp32()
{
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }
  String json = "{\"CARTAO SD\":";
  json += Status_SD;
  json += ",";
  json += "\"SPIFFS\":";
  json += Status_SPIFSS;
  json += ",";
  json += "\"RTC\":";
  json += Status_RTC;
  json += "}";

  server.send(200, "application/json", json);
}
void sensorsdata()
{
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }
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
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }
  String json = "{\"temperatura_c\":";
  json += readDSTemperatureC();
  json += "}";
  server.send(200, "application/json", json);
}
void json_Tempf()
{
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }
  String json = "{\"temperatura_f\":";
  json += readDSTemperatureF();
  json += "}";
  server.send(200, "application/json", json);
}
void json_umidade()
{
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }
  String json = "{\"umidade\":";
  json += onSensorChange();
  json += "}";
  server.send(200, "application/json", json);
}

void flowrate_cv()
{
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }
  String json = "{\"flowrate\":";
  json += flowrate_state;
  json += "}";
  server.send(200, "application/json", json);
}
void total_water()
{
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }
  String json = "{\"totalwater\":";
  json += total_water_state;
  json += "}";
  server.send(200, "application/json", json);
}
void datetime()
{
  // Verificar a chave de API na header
  if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
  {
    server.send(403, "text/plain", "Acesso negado");
  }
  String json = "{\"datatime\":";
  json += data_now();
  json += "}";
  server.send(200, "application/json", json);
}
void handleSN()
{
  server.send(200, "text / plain", "OK"); // Returns the HTTP response
}
// Rotas para Autenticaçao e Segurança no servido ESP32
void AuthentificationESP32()
{
  if (!server.authenticate(username, userPassword))
  {
    return server.requestAuthentication();
  }
  loadFromSPIFFS("/mwsn-app/index.html", "text/html");
  // Lógica do manipulador de solicitação para usuários autenticados
  // server.send(200, "text/plain", "Acesso permitido ao recurso protegido");

}
// void Requisicao()
// {
//   // Verificar a chave de API na header
//   if (!server.hasHeader("X-Api-Token") || server.header("X-Api-Token") != apiToken)
//   {
//     server.send(403, "text/plain", "Acesso negado");
//   }
//   server.send(200, "text/plain", "Acesso permitido ao recurso protegido");
// }

// Inicializa o Servidor para controla o braço e a bomba
void init_Server()
{
  // Configuração das Rotas do Servidor
  // Adiciona a função "handle_on_connect" quando o servidor estiver online
  server.on("/", HTTP_GET, handleRoot);

  // server.on("/autenticar", HTTP_GET, handleRoot);

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

  // Rota para Enviar os dados de todos os sensores:
  server.on("/sensor_node_data", HTTP_GET, sensorsdata);
  server.on("/config_esp32", HTTP_GET, config_esp32);

  // Rota para retornar data e hora

  server.on("/data_rtc", HTTP_GET, datetime);

  // Rota protegida por autenticação
  server.on("/login", HTTP_GET, AuthentificationESP32);
  // Rota protegida por token de API
  // server.on("/teste", HTTP_GET, Requisicao);

  //  Adiciona a função "handle_not_found" quando o servidor estiver offline
  server.onNotFound(handleNotFound);
  // here the list of headers to be recorded
  const char *headerkeys[] = {"User-Agent", "Cookie", "X-Api-Token"};
  size_t headerkeyssize = sizeof(headerkeys) / sizeof(char *);
  // ask server to track these headers
  server.collectHeaders(headerkeys, headerkeyssize);

  // Inicia o servidor
  server.begin();

  Serial.println("Server MWSN started");
}
