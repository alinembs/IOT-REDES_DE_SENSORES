#include <Wire.h>
#include <Arduino.h>


void receberDados(int quantidade) {
  while (Wire.available()) {
    char c = Wire.read();
    Serial.print("Dado recebido do Raspberry Pi: ");
    Serial.println(c);
    // Faça algo com o dado recebido, por exemplo, movimentar um servo
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8);                // Inicia o dispositivo I2C com endereço 8
  Wire.onReceive(receberDados);  // Configura a função de callback para receber dados
 
}

void loop() {
  delay(100);
  // Coloque qualquer lógica adicional aqui
}


