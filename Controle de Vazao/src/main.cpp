#include <Arduino.h>
#include "SPIFFS.h"
#include <WebServer.h>
#include <WiFi.h>

const char *ssid = "ESPWIFI";
const char *password = "1234567890";
WebServer server(80);

String flowrate_state = " ";
String total_water_state = " ";

#define PIN_SENSOR_FLUXO 13

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
boolean ledState = LOW;
float calibrationFactor = 6.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
float flowLitres;
float totalLitres;
//Funcao Auxiliar do Sensor de Vazao
void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}

//Inicia o Pinos do Relé e do Sensor de Vazão
void init_bomba_sensorfluxo()
{

  pinMode(PIN_SENSOR_FLUXO,INPUT_PULLUP);


  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_FLUXO), pulseCounter, FALLING);

}

void Controle_sensor_vazao()
{
currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;

    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();

    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    flowLitres = (flowRate / 60);

    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
    totalLitres += flowLitres;

    flowrate_state = String(flowRate);
    total_water_state = String(totalLitres);
    // Add the millilitres passed in this second to the cumulative total
    // flowrate_state = String(flowRate);
    // total_water_state = String(totalMilliLitres);
    // Print the flow rate for this second in litres / minute
  //   Serial.print("Flow rate: ");
  //   Serial.print(int(flowRate));  // Print the integer part of the variable
  //   Serial.print("L/min");
  //   Serial.print("\t");       // Print tab space

  //   // Print the cumulative total of litres flowed since starting
  //   Serial.print("Output Liquid Quantity: ");
  //   Serial.print(totalMilliLitres);
  //   Serial.print("mL / ");
  //   Serial.print(totalMilliLitres / 1000);
  //   Serial.println("L");
   }

}
// Manipulador para páginas não encontradas
void handleNotFound()
{
  server.send(404, "text/plain", "Página não encontrada");
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

// Iniciar Wifi no Modo Normal
void init_Wifi_NM()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  IPAddress serverIp;
  Serial.println("Host address resolved:");
  Serial.println(serverIp.toString());
  IPAddress myIP = WiFi.localIP();
  Serial.print("IP address: ");
  Serial.println(myIP);
}

void handleRoot()
{
  loadFromSPIFFS("/index.html", "text/html");
}

void flowrate_cv()
{
  server.send_P(200, "text/plain", flowrate_state.c_str());
}
void total_water()
{
  server.send_P(200, "text/plain", total_water_state.c_str());
}
void init_Server()
{

  server.on("/", HTTP_GET, handleRoot);
  server.on("/flowrate", HTTP_GET, flowrate_cv);
  // server.on("/temperaturef", HTTP_GET, sensor_TemperatureF);
  server.on("/total", HTTP_GET, total_water);
  server.onNotFound(handleNotFound);
  // Inicia o servidor
  server.begin();
  Serial.println("Server started");
}

void setup()
{

  Serial.begin(115200);
  initSPIFFS();
  init_Wifi_NM();
  init_Server();
  init_bomba_sensorfluxo();
}

void loop()
{

// currentMillis = millis();
//   if (currentMillis - previousMillis > interval) {
    
//     pulse1Sec = pulseCount;
//     pulseCount = 0;

//     // Because this loop may not complete in exactly 1 second intervals we calculate
//     // the number of milliseconds that have passed since the last execution and use
//     // that to scale the output. We also apply the calibrationFactor to scale the output
//     // based on the number of pulses per second per units of measure (litres/minute in
//     // this case) coming from the sensor.
//     flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
//     previousMillis = millis();

//     // Divide the flow rate in litres/minute by 60 to determine how many litres have
//     // passed through the sensor in this 1 second interval, then multiply by 1000 to
//     // convert to millilitres.
//     flowMilliLitres = (flowRate / 60) * 1000;
//     flowLitres = (flowRate / 60);

//     // Add the millilitres passed in this second to the cumulative total
//     totalMilliLitres += flowMilliLitres;
    
//     flowrate_state = String(flowRate);
//     total_water_state = String(totalMilliLitres);
//     // Print the flow rate for this second in litres / minute
//     // Serial.print("Flow rate: ");
//     // Serial.print(int(flowRate));  // Print the integer part of the variable
//     // Serial.print("L/min");
//     // Serial.print("\t");       // Print tab space

//     // // Print the cumulative total of litres flowed since starting
//     // Serial.print("Output Liquid Quantity: ");
//     // Serial.print(totalMilliLitres);
//     // Serial.print("mL / ");
//     // Serial.print(totalMilliLitres / 1000);
//     // Serial.println("L");

//   }
Controle_sensor_vazao();
server.handleClient();
delay(10);
}
