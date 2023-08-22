#include <Arduino.h>

#define AOUT_PIN 36 // ESP32 pin GIOP36 (ADC0) that connects to AOUT pin of moisture sensor
#define AOUT2_PIN 39 // ESP32 pin GIOP39 (ADC1_3) that connects to AOUT pin of moisture sensor
#define THRESHOLD 1000 // CHANGE YOUR THRESHOLD HERE


void setup() {
    Serial.begin(115200);
}

void loop() {

int value = analogRead(AOUT_PIN); // read the analog value from sensor
Serial.println("HUMIDITY 1 : ");
Serial.println(value);
int value2 = analogRead(AOUT2_PIN); // read the analog value from sensor
Serial.println("HUMIDITY 2: ");
Serial.println(value2);
delay(3000);

}

