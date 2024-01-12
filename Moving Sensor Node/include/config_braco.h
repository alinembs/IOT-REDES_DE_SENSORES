#include <ESP32Servo.h>

Servo servo_base;
Servo servo_horizontal;
Servo servo_vertical;

// GPIO the servo is attached to
static const int servoPin0 = 26;
static const int servoPin1 = 27;
static const int servoPin2 = 25;
//Inicia os pinos dos 3 Servos Motores
void init_braco_robotico()
{
    ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);

    servo_base.setPeriodHertz(50);
    servo_horizontal.setPeriodHertz(50);
    servo_vertical .setPeriodHertz(50); 
    servo_base.attach(servoPin0, 1000, 2000);
    servo_horizontal.attach(servoPin1, 1000, 2000);
    servo_vertical.attach(servoPin2, 1000, 2000);
  
}
  