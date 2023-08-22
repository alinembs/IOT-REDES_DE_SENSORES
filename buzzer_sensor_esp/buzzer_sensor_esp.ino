#define trig 25
#define echo  26

float trigPuls();
float media_pulse();
float dist_cma = 0;

int buzzer_pin      = 27;
int channel         = 0;
int frequence       = 2000;
int resolution      = 10;
unsigned long timer = millis();

TaskHandle_t dobitaobyte;
eTaskState statusOf;


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

void sirene(void *pvParameters){
    float sinVal;
    int   toneVal;
    for (byte t = 0; t<10;t++){
        for (byte x=0;x<180;x++){
            //converte graus em radianos
            sinVal = (sin(x*(3.1412/180)));
            //agora gera uma frequencia
            toneVal = 2000+(int(sinVal*100));
            //toca o valor no buzzer
            ledcWriteTone(channel,toneVal);
            //Serial.print("*");
            //atraso de 2ms e gera novo tom
            delay(4);
        }
    }
    ledcWriteTone(channel, 0);
    vTaskDelete(NULL);
}
void chamar_sirene()
{
   if (millis()-timer >10000){
    statusOf = eTaskGetState(dobitaobyte);
    if (statusOf == eReady){
      timer = millis();
      xTaskCreatePinnedToCore(sirene,"sirene", 10000, NULL, 1, &dobitaobyte,0);
    }
    
  }
   
}



void setup() {
  // put your setup code here, to run once:

pinMode(trig, OUTPUT);
pinMode(echo, INPUT);

digitalWrite(trig,LOW);
 ledcSetup(channel, frequence, resolution);
  ledcAttachPin(buzzer_pin, channel);
  timer = millis();
  Serial.begin(115200);
  xTaskCreatePinnedToCore(sirene,"sirene", 10000, NULL, 1, &dobitaobyte,0);


}

void loop() {

dist_cma = media_pulse();
delay(200);
 if (dist_cma <= 28){
   Serial.println("SIRENE TOCANDO...");
   chamar_sirene();
   delay(8000);
  }
  else { 
    Serial.print(dist_cma, 0);
    Serial.print("cm,  ");
    Serial.println();
  }
  delay(1000);     
}
