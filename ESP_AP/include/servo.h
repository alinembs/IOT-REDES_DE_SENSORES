#include <Arduino.h>
#include <Servo.h>

#define ServoPin 26

Servo myservo;

String estado_servo = " ";
String valor_servo = " ";


String writeSERVO(int i)
{
  String servo;
  if (i == 0)
  {
    servo = "PARADO";
   // Serial.print("Estado do Solo: ");
   // Serial.println(servo);
  }
  else
  {
    servo = "MOVIMENTO";
   // Serial.print("Estado do Solo: ");
   // Serial.println(servo);
  }
  
  return servo;
}