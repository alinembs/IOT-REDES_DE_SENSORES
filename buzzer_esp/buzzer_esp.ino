int buzzer_pin      = 26;
int channel         = 0;
int frequence       = 2000;
int resolution      = 10;
unsigned long timer = millis();

TaskHandle_t dobitaobyte;
eTaskState statusOf;

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
  
  ledcSetup(channel, frequence, resolution);
  ledcAttachPin(buzzer_pin, channel);
  timer = millis();
  Serial.begin(115200);
  xTaskCreatePinnedToCore(sirene,"sirene", 10000, NULL, 1, &dobitaobyte,0);
}

void loop() {

  Serial.println("SIRENE TOCANDO...");
  chamar_sirene();
  delay(8000);
}
