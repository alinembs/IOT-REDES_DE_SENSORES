#define trig 25
#define echo  26

float trigPuls();
float media_pulse();
float dist_cma = 0;

void setup() {
  // put your setup code here, to run once:

pinMode(trig, OUTPUT);
pinMode(echo, INPUT);

digitalWrite(trig,LOW);
Serial.begin(9600);

}

void loop() {

dist_cma = media_pulse();
Serial.println(dist_cma); 
 delay(200);
}

float trigPulse()
{
float pulse;
float dist_cm;
float erro = 3.5 ;
float dist_calibrada = 0;
digitalWrite(trig, HIGH);
delayMicroseconds(10);
digitalWrite(trig,LOW); 
 pulse = pulseIn(echo, HIGH, 200000);
 dist_cm = pulse/58.82;
 dist_calibrada = dist_cm + erro;

  return dist_calibrada;
}

float media_pulse()
{
 float leituraSum = 0;
 float resultado = 0 ;

for (int index = 0 ; index< 30; index++ ){

  delay(30);
  leituraSum +=trigPulse();

} 
  resultado = (float) leituraSum/30;
return resultado;
  
}
