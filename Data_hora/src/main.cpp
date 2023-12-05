#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>
RTC_DS3231 rtc;
void setup()
{
  Serial.begin(115200);
  Wire.begin();
  rtc.begin();
  rtc.adjust(DateTime(__DATE__, __TIME__));
}

void loop()
{

  DateTime now = rtc.now();

  // Formato da data: DD/MM/AAAA
  String data = String(now.year()) + "/" + String(now.month()) + "/" + String(now.day());

  // Formato da hora: HH:MM:SS
  String hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  String data_hora = data + ' ' + hora;
  // Exibir a data e hora no monitor serial
  Serial.print("Data e hora: ");
  Serial.println(data_hora);

  delay(5000); // Esperar 1 segundo antes de exibir novamente
}