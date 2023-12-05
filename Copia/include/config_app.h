#include <Arduino.h>

// INCLUDES DOS SENSORES
#include "temperature.h"
#include "humidity.h"
// INCLUDES DOS ATUADORES
#include "bomba.h"
#include "servo.h"



void IRRIGACAO()
{
  Serial.println("Iniciando Irrigação");
  estado_servo = writeSERVO(1);
  int pos = 0;
  // Armar o Braço
  for (pos = 0; pos <= 180; pos += 1)
  {

    myservo.write(pos);
    delay(15);
  }
  // Ligar o Motor
  Bomba_Agua("ON");
  delay(7000);
  // Desligar Motor
  Bomba_Agua("OFF");
  delay(2000);
  Serial.println("Finalizando a Irrigação");
  estado_servo = writeSERVO(2);
  // Voltar o Braço
  for (pos = 180; pos >= 0; pos -= 1)
  {                     // goes from 180 degrees to 0 degrees
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);          // waits 15ms for the servo to reach the position
  }

  // myservo.write(0);
  delay(1000);
  Serial.println("Irrigação Concluida");
  Serial.println("-------------------------");
  estado_servo = writeSERVO(0);
}

void mode_app(String mode)
{
  if ((mode == " ") || (mode == "0"))
  {
    Serial.println("-----------MODO AUTOMÁTICO ----------");
  }
  else if (mode == "1")
  {
    Serial.println("-----------MODO MANUAL ----------");
  }
  else
  {
    Serial.println("-----------ERRO DE MODO ----------");
  }
}