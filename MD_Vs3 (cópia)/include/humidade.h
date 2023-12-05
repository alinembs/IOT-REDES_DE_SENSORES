#include <Arduino.h>

#define limit 1295
#define min_moisture 4095
#define max_moisture 2800

#define otimo 75
#define regular 60
#define pessimo 50

#define AOUT_PIN 36

String solo = " ";
String humidity = " ";

// FUNÇOES

float Percent_Moisture(int value)
{
  int valor = 100 - ((value - max_moisture) * 100) / limit;
  if (valor < -1)
  {
    valor = 0;
  }
  if (valor > 100)
  {
    valor = 100;
  }
  return valor;
}


String writeSolo(float percent)
{
  String solo = "";

  if (percent > 75)
  {
    solo = "Positivo";
    // Serial.println(solo);
  }
  if ((percent < 75) & (percent > 50))
  {
    solo = "Regular";
    // Serial.println(solo);
  }
  if (percent < 50)
  {
    solo = "Crítico";
    // Serial.println(solo);
  }

  if (solo == "")
  {
    Serial.println("Failed to find ground state");
    return "--";
  }
  else
  {
    Serial.print("Estado do Solo: ");
    Serial.println(solo);
  }
  return solo;
}

String readDSHumidity()
{
  int value = analogRead(AOUT_PIN);
  Serial.print("Humidity: ");
  Serial.println(value);
  float percent = Percent_Moisture(value);
  solo = writeSolo(percent);
  if ((percent < -1) & (percent > 100))
  {
    Serial.println("Failed to read from Humidity sensor");
    return "--";
  }
  else
  {
    Serial.print("Humidity %: ");
    // percent = 0;
    Serial.println(percent);
  }
  return String(percent);
}