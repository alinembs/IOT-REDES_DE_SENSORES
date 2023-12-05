#include <Arduino.h>
#include "config.h"

void setup() {
  
  Serial.begin(115200);
  initSPIFFS();
  init_Wifi_NM();
  init_Server();
}

void loop() {
  
  // Clientes --------------
  server.handleClient();
  delay(1000);

}
