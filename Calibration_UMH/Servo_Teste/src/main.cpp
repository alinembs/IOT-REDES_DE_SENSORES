#include <Arduino.h>
#include <Servo.h>

#define THRESHOLD 1000 // CHANGE YOUR THRESHOLD HERE
#define ServoPin   26
#define ServoPin1   27

Servo myservo;

Servo myservo1;
void setup() {
    Serial.begin(115200);
    myservo.attach(ServoPin);
    myservo1.attach(ServoPin1);
}

void loop() {

    for(int posDegrees = 0; posDegrees <= 180; posDegrees++) {
        myservo.write(posDegrees);
        myservo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }

    for(int posDegrees = 180; posDegrees >= 0; posDegrees--) {
        myservo.write(posDegrees);
        myservo1.write(posDegrees);
        Serial.println(posDegrees);
        delay(20);
    }


  delay(100);

}
