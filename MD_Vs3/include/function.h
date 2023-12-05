#include <Arduino.h>
#include "configwifi.h"

void IRRIGACAO()
{
  estado_servo = writeSERVO(1);
  int pos = 0;
  // Armar o Braço
  for (pos = 0; pos <= 180; pos += 1)
  {

    myservo.write(pos);
    delay(15);
  }
  humidity = readDSHumidity();
  // Ligar o Motor
  int limite = 0;
  // myservo.write(pos);
  Serial.println("Irrigando...");
  while (limite != 1)
  {
    int value = analogRead(AOUT_PIN);
    float percent = Percent_Moisture(value);
    humidity = readDSHumidity();
    // Serial.println("Percentual:");
    // Serial.println(percent,2);
    // Serial.print("Moisture value: ");
    // Serial.println(value);

    delay(5000);
    if (percent > otimo)
    {
      limite = 1;
    }
  }
  // Desligar Motor

  Serial.println("Finalizando a Irrigação");
  delay(5000);
  estado_servo = wchar_t(2);
  // Voltar o Braço
  for (pos = 180; pos >= 0; pos -= 1)
  {                     // goes from 180 degrees to 0 degrees
    myservo.write(pos); // tell servo to go to position in variable 'pos'
    delay(15);          // waits 15ms for the servo to reach the position
  }

  // myservo.write(0);
  Serial.println("Irrigação Concluida");
  Serial.println("-------------------------");
  estado_servo = writeSERVO(0);
}

void main_loop()
{
  temperatureC = readDSTemperatureC();
  temperatureF = readDSTemperatureF();
  humidity = readDSHumidity();
  Serial.println("ESTADO DO SERVO:" + estado_servo);
  Serial.println("ESTADO DO BOMBA:" + estado_bomba);
  if ((MODO_ATUAL == " ") || (MODO_ATUAL == "0"))
  {

    int value = analogRead(AOUT_PIN);
    float percent = Percent_Moisture(value);
    if (percent <= 69)
    {
      Serial.println("INICIO DA IRRIGAÇAO");
      estado_servo = writeSERVO(1);
      int pos = 0;
      // Armar o Braço
      for (pos = 0; pos <= 180; pos += 1)
      {

        myservo.write(pos);
        delay(15);
      }
      humidity = readDSHumidity();
      Bomba_Agua("ON");
      // Ligar o Motor
      int limite = 0;
      // myservo.write(pos);
      Serial.println("Irrigando...");
      while (limite != 1)
      {
        int value = analogRead(AOUT_PIN);
        float percent = Percent_Moisture(value);
        humidity = readDSHumidity();
        // Serial.println("Percentual:");
        // Serial.println(percent,2);
        // Serial.print("Moisture value: ");
        // Serial.println(value);

        delay(5000);
        if (percent > otimo)
        {
          limite = 1;
        }
      }
      Bomba_Agua("OFF");
      // Desligar Motor

      Serial.println("Finalizando a Irrigação");
      delay(5000);
      estado_servo = writeSERVO(1);
      // Voltar o Braço
      for (pos = 180; pos >= 0; pos -= 1)
      {                     // goes from 180 degrees to 0 degrees
        myservo.write(pos); // tell servo to go to position in variable 'pos'
        delay(15);          // waits 15ms for the servo to reach the position
      }

      // myservo.write(0);
      Serial.println("Irrigação Concluida");
      Serial.println("-------------------------");
      estado_servo = writeSERVO(0);
    }
  }
}
