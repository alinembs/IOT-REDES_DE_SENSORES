#include <Arduino.h>

#define rele 27


String estado_bomba = " ";

void Bomba_Agua(String state)
{
  if (state == "OFF")
  {
    digitalWrite(rele, LOW);
    estado_bomba = "DESLIGADO";
  }
  else if (state == "ON")
  {
    digitalWrite(rele, HIGH);
    estado_bomba = "LIGADO";
  }
  else
  {
    digitalWrite(rele, LOW);
    estado_bomba = "DESLIGADO";
    estado_bomba = "--";
  }
}