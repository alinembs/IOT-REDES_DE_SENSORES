#include <Arduino.h>
#include <Servo.h>
#define rele 27
void setup() {
  // put your setup code here, to run once:
  pinMode(rele,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(rele,HIGH);
  delay(3000);
  digitalWrite(rele,LOW);
  delay(3000);
}