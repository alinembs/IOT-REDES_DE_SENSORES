// INCLUDE DOS CONFIG
#include <ArduinoJson.h>
#include "config_storage.h"
#include "config_app.h"

// INFO dos button

String MODO_ATUAL = " ";

const char *PARAM_MODE = "state";
const char *PARAM_FILE = "value";

// Manipulador para a página principal
void handleRoot()
{
  //loadFromSD(SD, "/planta-vision-app/home.html", "text/html");
  loadFromSPIFFS("/planta-vision-app/home.html","text/html");
}
void handleRootcss()
{
  loadFromSD(SD, "/planta-vision-app/css/style.css", "text/css");
  // loadFromSPIFFS("/style.css","text/css");
}

void handleRootjs()
{
  //loadFromSD(SD, "/planta-vision-app/js/script.js", "text/js");
  loadFromSPIFFS("/planta-vision-app/js/script.js","text/js");
}

/////////////////route dir css /////////////////////////////

void handleRootbootstrap_1css()
{
  loadFromSD(SD, "/planta-vision-app/css/bootstrap.min.css", "text/css");
  // loadFromSPIFFS("/script.js","text/js");
}
void handleRootbootstrap_2css()
{
  loadFromSD(SD, "/planta-vision-app/css/bootstrap.css", "text/css");
  // loadFromSPIFFS("/script.js","text/js");
}
void handleRootbootstrap_3css()
{
  loadFromSD(SD, "/planta-vision-app/css/bootstrap.min.css.map", "text/css");
  // loadFromSPIFFS("/script.js","text/js");
}
///////////// route dir js //////////////////////////////////

void handleRootbootstrap_1js()
{
  loadFromSD(SD, "/planta-vision-app/js/bootstrap.bundle.min.js", "text/js");
}
void handleRootbootstrap_2js()
{
  loadFromSD(SD, "/planta-vision-app/js/bootstrap.bundle.min.js.map", "text/js");
}
void handleRootbootstrap_3js()
{
  loadFromSD(SD, "/planta-vision-app/js/index.umd.js", "text/js");
}

///////////////////route dir icon/////////////////////////
void handleRootlin()
{
  loadFromSD(SD, "/planta-vision-app/icon/linkedin.svg", "image/svg");
}
void handleRootgit()
{
  loadFromSD(SD, "/planta-vision-app/icon/github.svg", "image/svg");
}
void handleRooticon()
{
  loadFromSD(SD, "/planta-vision-app/icon/icons8-robo-60.png", "image/png");
}

/////////////////////////rout dir img///////////////////////////
void handleRootplantacard()
{

  loadFromSD(SD, "/planta-vision-app/img/41021.jpg", "image/jpg");
}


void handleRootivan()
{
  loadFromSD(SD, "/planta-vision-app/img/fundo.png", "image/png");
  
}
void handleRootluis()
{

  loadFromSD(SD, "/planta-vision-app/img/luis-tosta-SVeCm5KF_ho-unsplash.jpg", "image/jpg");
}

void handleRootmilada()
{
  loadFromSD(SD, "/planta-vision-app/img/milada-vigerova-A-1Tfla6kmE-unsplash.jpg", "image/pg");
}

void handleRootraimo()
{
  loadFromSD(SD, "/planta-vision-app/img/raimond-klavins-MOzwsN74YAs-unsplash.jpg", "image/jpg");
}

///////////////////////route dir chart/////////////////////

void handleRoothgc1()
{
  loadFromSD(SD, "/planta-vision-app/chart/highcharts_stock/highstock.js", "text/js");
}
void handleRoothgc2()
{

  loadFromSD(SD, "/planta-vision-app/chart/highcharts_stock/highstock.js.map", "text/js");
}

void handleRoothgc3()
{
  loadFromSD(SD, "/planta-vision-app/chart/highcharts_stock/highstock.src.js", "text/js");
}

//////////////////////route sensor variables ///////////////////
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
  server.send_P(200, "text/plain", onSensorChange().c_str());
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
/////////////////////route function /////////////////////////
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
/*void handleServoArg()
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

  server.send(200, "text / plain", "OK"); // Returns the HTTP response
}
*/
/*
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

  //Bomba_Agua(message);
  server.send(200, "text / plain", "OK"); // Returns the HTTP response
}
*/
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
  // Configuração das Rotas do Servidor

  // Adiciona a função "handle_on_connect" quando o servidor estiver online

  server.on("/", HTTP_GET, handleRoot);
  server.on("/planta-vision-app/css/style.css", HTTP_GET, handleRootcss);
  server.on("/planta-vision-app/js/script.js", HTTP_GET, handleRootjs);

  // route css

  server.on("/planta-vision-app/css/bootstrap.css", HTTP_GET, handleRootbootstrap_1css);
  server.on("/planta-vision-app/css/bootstrap.min.css", HTTP_GET, handleRootbootstrap_2css);
  server.on("/planta-vision-app/css/bootstrap.min.css.map", HTTP_GET, handleRootbootstrap_3css);

  // route jss
  server.on("/planta-vision-app/js/bootstrap.bundle.min.js", HTTP_GET, handleRootbootstrap_1js);
  server.on("/planta-vision-app/js/bootstrap.bundle.min.js.map", HTTP_GET, handleRootbootstrap_2js);
  server.on("/planta-vision-app/js/index.umd.js", HTTP_GET, handleRootbootstrap_3js);

  // route charts

  server.on("/planta-vision-app/chart/highcharts_stock/highstock.js", HTTP_GET, handleRoothgc1);
  server.on("/planta-vision-app/chart/highcharts_stock/highstock.js.map", HTTP_GET, handleRoothgc2);
  server.on("/planta-vision-app/chart/highcharts_stock/highstock.src.js", HTTP_GET, handleRoothgc3);
  // route  img

  server.on("/planta-vision-app/img/41021.jpg", HTTP_GET, handleRootplantacard);
  server.on("/planta-vision-app/img/raimond-klavins-MOzwsN74YAs-unsplash.jpg", HTTP_GET, handleRootraimo);
  server.on("/planta-vision-app/img/luis-tosta-SVeCm5KF_ho-unsplash.jpg", HTTP_GET, handleRootluis);
  server.on("/planta-vision-app/img/milada-vigerova-A-1Tfla6kmE-unsplash.jpg", HTTP_GET, handleRootmilada);
  
  
  // route icon
  server.on("/planta-vision-app/icon/github.svg", HTTP_GET, handleRootlin);
  server.on("/planta-vision-app/icon/icons8-robo-60.png", HTTP_GET, handleRooticon);
  server.on("/planta-vision-app/icon/linkedin.svg", HTTP_GET, handleRootgit);

  // route da image fundo  - maior - arquivo
  server.on("/planta-vision-app/img/fundo.png", HTTP_GET, handleRootivan);
  // route sensor
  server.on("/temperaturec", HTTP_GET, sensor_TemperatureC);
  //server.on("/temperaturef", HTTP_GET, sensor_TemperatureF);
  server.on("/umidade", HTTP_GET, sensor_humidity);
  //server.on("/solo", HTTP_GET, state_solo);
  //server.on("/servo", HTTP_GET, state_servo);
  //server.on("/butt_bomba", HTTP_GET, state_bomb);

  // Funções de Pegar dados Web
  //server.on("/update", HTTP_GET, handleModeArg);
  // Funções para Mandar o RaspiBerry Pi controlor o Braco e ou Bomba
  //server.on("/slider", HTTP_POST, handle_controle_braco);
  //server.on("/bomba", HTTP_GET, handle_controle_bomba);

  // Função para Baixar dados cvs
  server.on("/", HTTP_POST, File_Download);
  // Chamdas extras
  server.on("/checkpoint", HTTP_GET, salve_data);
  // server.on("/favicon.ico", HTTP_GET,handleSN);
  // Chamadas para se comunicar com a API
  server.on("/bd_save", HTTP_POST, handle_conect_api);
  //  Adiciona a função "handle_not_found" quando o servidor estiver offline
  server.onNotFound(handleNotFound);
  // Inicia o servidor
  server.begin();

  Serial.println("Server started");
}
