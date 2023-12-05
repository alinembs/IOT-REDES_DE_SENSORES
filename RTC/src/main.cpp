#include <Arduino.h>
#include <SPI.h>   // para o display OLED
#include <Wire.h>  // para o Modulo RTC I2C
#include <RTClib.h>

RTC_DS3231 rtc;
void init_RTC()
{
  Wire.begin();
  if (!rtc.begin())
  {
    Serial.println("Não foi possível encontrar RTC");
    //while(1);
  }
else{
  Serial.println("Foi possível encontrar RTC");
}
}
void setup() 
{
  Serial.begin(115200);
  Serial.println("Inicializando Aplicação");
  delay(1000);
  // Iniciar o RTC - Data e Hora
  init_RTC();
  
}

void loop()
{
 //Serial.println("Testes no Sistema");

}