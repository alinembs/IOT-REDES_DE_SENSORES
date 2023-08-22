
const int output = 25;
const int buttonPin = 26;
int buzzer_pin      = 27;
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




// Variables will change:
int ledState = LOW;          // the current state of the output pin
int buttonState = HIGH;            
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  pinMode(buttonPin, INPUT);
  ledcSetup(channel, frequence, resolution);
  ledcAttachPin(buzzer_pin, channel);
  timer = millis();
   xTaskCreatePinnedToCore(sirene,"sirene", 10000, NULL, 1, &dobitaobyte,0);
}

void loop() {
  
  int reading = digitalRead(buttonPin);
  
   if(reading == buttonState )
   {  
     if(ledState == LOW)
      {ledState = !ledState;
       chamar_sirene();
       delay(8000);
      }
   
  
   else{  
      ledState = !ledState;
      }
   }
   digitalWrite(output, ledState);
 delay(200);
}
