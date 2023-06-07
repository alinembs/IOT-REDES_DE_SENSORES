// ALINE MARIANA BARROS SILVA
// ALGORITMO PARA MONITORAMENTO DO SOLO
// BIBLIOTECAS

#include "configwifi.h"

void setup()
{

  Serial.begin(115200);
  Serial.println();
  delay(1000);
  // TIME

  // INCIANDO AS PORTAS
  Serial.println("Inicializando as Portas /PIN");
  myservo.attach(ServoPin);
  pinMode(rele, OUTPUT);
  sensors.begin();

  // TEMPERATURE
  temperatureC = readDSTemperatureC();
  temperatureF = readDSTemperatureF();

  // Humidity

  humidity = readDSHumidity();

  // Servo Bomb
  Bomba_Agua("OFF");

  estado_servo = writeSERVO(0);
  // INCIALIZAR SPIFF e SD CARD
  initSPIFFS();
  initSDCard();
  // INICIALIZAR WIFI
  initWifi();

  // INICIALIZAR SERVER
  initServer();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
 /* Serial.println("MONITORAMENTO ----- AGRICULTURA DE PRECISÃO");
  if (solo == "Crítico" || solo == "Regular")
  {
    IRRIGACAO();
  }*/
  delay(10000);
}
