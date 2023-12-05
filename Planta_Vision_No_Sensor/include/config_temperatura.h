#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

String temperatureC = "";
String temperatureF = "";

String readDSTemperatureC()
{

  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC == -127.00)
  {
    // Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  }
  else
  {
   // Serial.print("Temperature Celsius: ");
    //Serial.println(tempC);
  }
  temperatureC = String(tempC);
  return String(tempC);
}

String readDSTemperatureF()
{

  sensors.requestTemperatures();
  float tempF = sensors.getTempFByIndex(0);

  if (int(tempF) == -196)
  {
    // Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  }
  else
  {
    // Serial.print("Temperature Fahrenheit: ");
    // Serial.println(tempF);
  }
  temperatureF = String(tempF);
  return String(tempF);
}