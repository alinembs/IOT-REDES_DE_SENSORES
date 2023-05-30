#include <Arduino.h>
#include <Servo.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#define AOUT_PIN 36 // ESP32 pin GIOP36 (ADC0) that connects to AOUT pin of moisture sensor
// GPIO where the DS18B20 is connected to
#define limit 1500
#define min_moisture 4095
#define max_moisture 2400

float Percent_Moisture(int value)
{
int valor = 100 - ((value - max_moisture)*100)/limit;
if(valor < -1)
{valor = 0;}
return valor;
}

const int oneWireBus = 4;     
// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(115200);
  // Start the DS18B20 sensor
  sensors.begin();
  myservo.attach(26);  // attaches the servo on pin 13 to the servo object
}

void loop() {

  //myservo.write(0);

  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  int value = analogRead(AOUT_PIN); // read the analog value from sensor
  float percent = Percent_Moisture(value);

  Serial.println("Dados do Solo:");
  Serial.print(temperatureC);
  Serial.println("ºC");
  Serial.print(temperatureF);
  Serial.println("ºF");
  Serial.println("Percentual:");
  Serial.println(percent,2);
  Serial.print("Moisture value: ");
  Serial.println(value);
  Serial.println("-------------------------");
  delay(4000);
 
  if( percent < 60)
  {
    Serial.print("Movimento Servo");  
int pos = 0;
//Armar o Braço
for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

//Ligar o Motor 
 int limite=0;
 myservo.write(pos);
 while(limite < 90)
  { 
   
    int value = analogRead(AOUT_PIN); 
    float percent = Percent_Moisture(value);
    Serial.println("Irrigando...");
    Serial.println("Percentual:");
    Serial.println(percent,2);
    Serial.print("Moisture value: ");
    Serial.println(value);
    delay(3000);
    if(percent  > 70)
    {
      limite = 91;
    }
  }
  //Desligar Motor
 Serial.println("Finalizando a Irrigação");
 delay(3000);
//Voltar o Braço
for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  //myservo.write(0);
  Serial.println("Irrigação Concluida");
  Serial.println("-------------------------");
  }
  
}

