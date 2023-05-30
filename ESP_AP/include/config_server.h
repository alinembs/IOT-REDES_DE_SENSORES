// INCLUDE DOS CONFIG

#include "config_storage.h"
#include "config_app.h"

// INFO dos button

String MODO_ATUAL = " ";
String BOMBA_STATUS = "OFF";

const char *PARAM_MODE = "state";
const char *PARAM_SERVO = "value";
const char *PARAM_BOMBA = "relay";

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
  loadFromSD(SD, "/img/markus-spiske-sFydXGrt5OA-unsplash.jpg", "image/jpg");
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
    estado_servo = writeSERVO(0);
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
  server.on("/img/markus-spiske-sFydXGrt5OA-unsplash.jpg", HTTP_GET, handleRootheader);
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
  server.on("/update", HTTP_GET, handleBombArg);

  // Adiciona a função "handle_not_found" quando o servidor estiver offline
  server.onNotFound(handleNotFound);

  // Route to load style.css and script.js file
  // Inicia o servidor
  server.begin();

  Serial.println("Server started");
}
