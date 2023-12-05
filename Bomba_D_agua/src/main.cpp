#include <Arduino.h>

const int relayPin = 14; // Pino de controle do relé

void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
}

void loop() { 

  digitalWrite(relayPin, HIGH); // Liga o relé e a bomba
  Serial.println("Ligando Relé");
  delay(10000); // Aguarda 10 segundos 

  digitalWrite(relayPin, LOW); // Desliga o relé e a bomba
  Serial.println("Desligando Relé");
  delay(5000); // Aguarda 5 segundos
}
