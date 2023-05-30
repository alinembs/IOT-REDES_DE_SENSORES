#include "application.h"

void setup()
{

  Serial.begin(115200);
  Serial.println();
  sensors.begin();
  init_App();
}

void loop(void)
{
  // Clientes --------------
  server.handleClient();
  delay(5); // allow the cpu to switch to other tasks
}