// INCLUDE DOS CONFIG
#include <ArduinoJson.h>
#include "config_storage.h"
#include "config_app.h"

// INFO dos button

String MODO_ATUAL = " ";
String BOMBA_STATUS = "OFF";

const char *PARAM_MODE = "state";
const char *PARAM_SERVO = "value";
const char *PARAM_BOMBA = "relay";
const char *PARAM_FILE = "value";

// VARIAVEIS PARA SE COMUNICAR COM API DO BANCO DE DADOS
const char *apiKey = "API_TESTE";
const char *apiEndpoint = "http://192.168.83.30:5000/adicionar-dados ";

// MANIPULADOPR PARA ENVIAR OS DADOS PARA BASE DE DADOS
void handle_conect_api()
{
  data_now();
  if (server.method() == HTTP_POST)
  {

    DynamicJsonDocument jsonDoc(256);
    jsonDoc["data_hora"] = data_hora;
    jsonDoc["temperatura_c"] = temperatureC;
    jsonDoc["temperatura_f"] = temperatureF;
    jsonDoc["umidade"] = humidity;
    String requestBody;
    serializeJson(jsonDoc, requestBody);

    HTTPClient http;
    http.begin(apiEndpoint);
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
  else
  {
    server.send(405, "text/html", "<h1>Método não permitido</h1>");
  }
}

// Manipulador para a página principal
void handleRoot()
{
  loadFromSD(SD, "/index.html", "text/html");
  // loadFromSPIFFS("/index.html","text/html");
}
void handleRootcss()
{
  loadFromSD(SD, "/style.css", "text/css");
  // loadFromSPIFFS("/style.css","text/css");
}

void handleRootjs()
{
  loadFromSD(SD, "/script.js", "text/js");
  // loadFromSPIFFS("/script.js","text/js");
}

void handleRoothc()
{
  loadFromSD(SD, "/components/highcharts.js", "text/js");
}
void handleRootjq()
{
  loadFromSD(SD, "/components/jquery.min.js", "text/js");
}
void handleRootlin()
{
  loadFromSD(SD, "/img/lin1.png", "image/png");
}
void handleRootheader()
{
  //loadFromSD(SD, "/img/markus-spiske-sFydXGrt5OA-unsplash.jpg", "image/jpg");
  loadFromSD(SD, "/img/7713.jpg", "image/jpg");
}

void handleRootgit()
{
  loadFromSD(SD, "/img/git2.png", "image/png");
}
void sensor_TemperatureF()
{
  server.send_P(200, "text/plain", readDSTemperatureF().c_str());
}
void sensor_TemperatureC()
{
  server.send_P(200, "text/plain", readDSTemperatureC().c_str());
}
void sensor_humidity()
{
  server.send_P(200, "text/plain", readDSHumidity().c_str());
}
void state_servo()
{
  server.send_P(200, "text/plain", estado_servo.c_str());
}
void state_solo()
{
  server.send_P(200, "text/plain", solo.c_str());
}
void state_bomb()
{
  server.send_P(200, "text/plain", estado_bomba.c_str());
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

void handleModeArg()
{

  String message = "";

  if (server.arg(PARAM_MODE) == "")
  { // Parameter not found

    message = "Argument not found";
  }
  else
  { // Parameter found

    message += server.arg(PARAM_MODE); // Gets the value of the query parameter
  }
  mode_app(message);
  server.send(200, "text / plain", "OK"); // Returns the HTTP response
}
void handleServoArg()
{

  String message = "";

  if (server.arg(PARAM_SERVO) == "")
  { // Parameter not found

    message = "Argument not found";
    // estado_servo = writeSERVO(0);
  }
  else
  { // Parameter found

    message += server.arg(PARAM_SERVO); // Gets the value of the query parameter
    myservo.write(message.toInt());
    estado_servo = writeSERVO(1);
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
  }

  Bomba_Agua(message);
  server.send(200, "text / plain", "OK"); // Returns the HTTP response
}
void handleSN()
{
  server.send(200, "text / plain", "OK"); // Returns the HTTP response
}
void File_Download()
{
  // This gets called twice, the first pass selects the input, the second pass then processes the command line arguments
  /*String file = "";
  if (server.arg(PARAM_FILE) == "")
  { // Arguments were received
    file = "Argument not found";
  }
  else
  {
    file += server.arg(PARAM_FILE);
    SD_file_download(file);
  }
  Serial.println(file);*/
  if (server.args() > 0) // Arguments were received, ignored if there are not arguments
  {
    // Serial.println(server.arg(0));

    String Order = server.arg(0);
    // Serial.println(Order);

    if (Order.indexOf("download") >= 0)
    {
      Order.remove(0, 10);
      SD_file_download(Order);
      Serial.println("Download Feito com Sucesso!");
    }
  }
}

void salve_data()
{
  data_now();
  String data;
  data = data_hora + ";" +
         temperatureC + ";" + temperatureF + " ;" + humidity;
  appendFile(SD, "/teste.csv", data.c_str());
  server.send(200, "text / plain", "OK"); // Returns the HTTP response
}
//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

void init_Server()
{

  // Adiciona a função "handle_on_connect" quando o servidor estiver online

  server.on("/", HTTP_GET, handleRoot);
  server.on("/style.css", HTTP_GET, handleRootcss);
  server.on("/script.js", HTTP_GET, handleRootjs);
  // Funcões Adicionais
  server.on("/components/highcharts.js", HTTP_GET, handleRoothc);
  server.on("/components/jquery.min.js", HTTP_GET, handleRootjq);
  server.on("/img/lin1.png", HTTP_GET, handleRootlin);
  //server.on("/img/markus-spiske-sFydXGrt5OA-unsplash.jpg", HTTP_GET, handleRootheader);
  server.on("/img/7713.jpg", HTTP_GET, handleRootheader);
  server.on("/img/git2.png", HTTP_GET, handleRootgit);
  // Funções dos Sensores e Atuadores
  server.on("/temperaturec", HTTP_GET, sensor_TemperatureC);
  server.on("/temperaturef", HTTP_GET, sensor_TemperatureF);
  server.on("/umidade", HTTP_GET, sensor_humidity);
  server.on("/solo", HTTP_GET, state_solo);
  server.on("/servo", HTTP_GET, state_servo);
  server.on("/butt_bomba", HTTP_GET, state_bomb);

  // Funções de Pegar dados Web
  server.on("/update", HTTP_GET, handleModeArg);
  server.on("/slider", HTTP_GET, handleServoArg);
  server.on("/bomba", HTTP_GET, handleBombArg);

  // Função para Baixar dados cvs
  server.on("/", HTTP_POST, File_Download);
  // Chamdas extras
  server.on("/checkpoint", HTTP_GET, salve_data);
  // server.on("/favicon.ico", HTTP_GET,handleSN);
  // Chamadas para se comunicar com a API
  server.on("/bd_save",HTTP_POST,handle_conect_api);
  //  Adiciona a função "handle_not_found" quando o servidor estiver offline
  server.onNotFound(handleNotFound);

  // Route to load style.css and script.js file
  // Inicia o servidor
  server.begin();

  Serial.println("Server started");
}
