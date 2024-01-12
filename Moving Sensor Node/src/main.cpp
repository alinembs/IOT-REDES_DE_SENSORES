#include <Arduino.h>
#include "config_data_hora_sd.h"
 

// unsigned long lastTime = 0;
// // Timer set to 10 minutes (600000)
// //unsigned long timerDelay = 600000;
// // Set timer to 5 seconds (5000)
// unsigned long timerDelay = 5000;

void setup() {

Serial.begin(115200);

init_RTC();
initSDCard();
initSPIFFS();
init_bomba_sensorfluxo();
init_braco_robotico();
init_Wifi_NM();
init_Server();

sensors.begin();
Serial.println("Moving Sensor Node");
Serial.println("Aplicação Iniciada!");

}

void loop() {

 Controle_sensor_vazao();
 server.handleClient();
 delay(10);
//   if ((millis() - lastTime) > timerDelay) {

//     sendSensorData();
//  lastTime = millis();
// }
}


