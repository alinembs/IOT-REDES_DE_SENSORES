#include <Arduino.h>


#define PIN_BOMBA 14
#define PIN_SENSOR_FLUXO 12

String flowrate_state = " ";
String total_water_state = " ";

long currentMillis = 0;
long previousMillis = 0;
int interval = 1000;
float calibrationFactor = 6.5;
volatile byte pulseCount;
byte pulse1Sec = 0;
float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
float flowLitres;
float totalLitres;
//Funcao Auxiliar do Sensor de Vazao
void IRAM_ATTR pulseCounter()
{
  pulseCount++;
}


//Inicia o Pinos do Relé e do Sensor de Vazão
void init_bomba_sensorfluxo()
{
  pinMode(PIN_BOMBA, OUTPUT);
  pinMode(PIN_SENSOR_FLUXO,INPUT_PULLUP);


  pulseCount = 0;
  flowRate = 0.0;
  flowMilliLitres = 0;
  totalMilliLitres = 0;
  previousMillis = 0;

  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_FLUXO), pulseCounter, FALLING);

}

void Controle_Bomba(String status)
{
if (status == "1")
{
  digitalWrite(PIN_BOMBA, HIGH); // Liga o relé e a bomba
  Serial.println("Ligando Relé");
}
if (status == "0")
{
  digitalWrite(PIN_BOMBA, LOW); // Desliga o relé e a bomba
  Serial.println("Desligando Relé");
}

}


void Controle_sensor_vazao()
{
currentMillis = millis();
  if (currentMillis - previousMillis > interval) {
    
    pulse1Sec = pulseCount;
    pulseCount = 0;

    // Because this loop may not complete in exactly 1 second intervals we calculate
    // the number of milliseconds that have passed since the last execution and use
    // that to scale the output. We also apply the calibrationFactor to scale the output
    // based on the number of pulses per second per units of measure (litres/minute in
    // this case) coming from the sensor.
    flowRate = ((1000.0 / (millis() - previousMillis)) * pulse1Sec) / calibrationFactor;
    previousMillis = millis();

    // Divide the flow rate in litres/minute by 60 to determine how many litres have
    // passed through the sensor in this 1 second interval, then multiply by 1000 to
    // convert to millilitres.
    flowMilliLitres = (flowRate / 60) * 1000;
    flowLitres = (flowRate / 60);
    // Add the millilitres passed in this second to the cumulative total
    totalMilliLitres += flowMilliLitres;
    totalLitres += flowLitres;
    flowrate_state = String(flowRate);
    total_water_state = String(totalLitres);
   
  }

    
}