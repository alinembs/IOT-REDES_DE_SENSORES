// ALINE MARIANA BARROS SILVA
// ALGORITMO PARA MONITORAMENTO DO SOLO
// BIBIOTECAS

#include "function.h"


void setup()
{

  Serial.begin(115200);
  Serial.println();

  // INCIANDO AS PORTAS

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
  //INCIALIZAR SPIFF
 
  // INICIALIZAR WIFI
  initWifi();
 
  //INICIALLIZAR SERVER

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void loop()
{
main_loop();

delay(3000);
}