#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>

#include <Preferences.h>
Preferences preferences;

RTC_DS3231 rtc;

void setup()
{
  Serial.begin(115200);
  preferences.begin("my-app", false);

  Wire.begin();
  if (!rtc.begin())
  {
    Serial.println("Não foi possível encontrar RTC");
    while (1)
      ;
  }
  bool data_modulo = preferences.getBool("data_modulo",false);
  if ((data_modulo == false) || (rtc.lostPower() == true))
  {
  rtc.adjust(DateTime(__DATE__, __TIME__));
  Serial.println("Data e hora definidas!");
  data_modulo = true;
  }
  preferences.putBool("data_modulo",data_modulo);
  preferences.end();
}

void loop()
{
  DateTime now = rtc.now();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();

  delay(1000);
}
