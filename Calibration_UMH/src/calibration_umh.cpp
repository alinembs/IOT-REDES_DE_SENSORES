#include <Arduino.h>
#define AOUT_PIN 36 // ESP32 pin GIOP36 (ADC0) that connects to AOUT pin of moisture sensor
#define limit 1400
#define min_moisture 4095
#define max_moisture 2600

float Percent_Moisture(int value)
{
int valor = 100 - ((value - max_moisture)*100)/limit;
return valor;
}
void setup() {
  Serial.begin(115200);
}

void loop() {
  int value = analogRead(AOUT_PIN); // read the analog value from sensor

  Serial.print("Moisture value: ");
  Serial.println(value);
  float percent = Percent_Moisture(value);
  Serial.println("Percentual:");
  Serial.println(percent,3);
  delay(5000);
}
//UMD 2500 100%
// UMD 4095  0%
// 1595 
//2700 ? 
