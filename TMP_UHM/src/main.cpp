#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define AOUT_PIN 36 // ESP32 pin GIOP36 (ADC0) that connects to AOUT pin of moisture sensor
// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;
#define limite 1295
#define min_moisture 4095
#define max_moisture 2800
#define wet 2800
#define dry 4095
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);

String onSensorChange()
{
  float sensor;
  sensor = analogRead(AOUT_PIN);
  sensor = map(sensor, dry, wet, 0, 100);

  // humidity = String(sensor);
  return String(sensor);
}
void setup()
{
  // Start the Serial Monitor
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
}

void loop()
{

  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print(temperatureC);
  Serial.println("ºC");
  // Serial.print(temperatureF);
  // Serial.println("ºF");
  int value = analogRead(AOUT_PIN);           // read the analog value from sensor
  float tensao = (float)value / 4095.0 * 3.3; // Supondo 3,3V como tensão de referência
  Serial.print("Moisture value: ");
  Serial.println(value);
  Serial.println("Umidade:");
  Serial.println(onSensorChange());
  Serial.println("Tensão:");
  Serial.println(tensao);

  delay(5000);
}