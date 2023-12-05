#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#include <Wire.h>
#include <RTClib.h>
#include <SPI.h>

#include <Preferences.h>

#include "FS.h"
#include "SD.h"
#include "SPI.h"

Preferences preferences;

RTC_DS3231 rtc;

#define AOUT_PIN 36 // ESP32 pin GIOP36 (ADC0) that connects to AOUT pin of moisture sensor
// GPIO where the DS18B20 is connected to
const int oneWireBus = 4;

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(oneWireBus);

// Pass our oneWire reference to Dallas Temperature sensor
DallasTemperature sensors(&oneWire);
void data_hora_now()
{
  DateTime now = rtc.now();

  // Formato da data: DD/MM/AAAA
  String data = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day());
  Serial.println(data);
  // Formato da hora: HH:MM:SS
  String hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  Serial.println(hora);
  // data_hora = data + ' ' + hora;
}
void initSDCard()
{
  if (!SD.begin(5))
  {
    Serial.println("Card Mount Failed");
    return;
  }
  uint8_t cardType = SD.cardType();

  if (cardType == CARD_NONE)
  {
    Serial.println("No SD card attached");
    return;
  }

  Serial.print("SD Card Type: ");
  if (cardType == CARD_MMC)
  {
    Serial.println("MMC");
  }
  else if (cardType == CARD_SD)
  {
    Serial.println("SDSC");
  }
  else if (cardType == CARD_SDHC)
  {
    Serial.println("SDHC");
  }
  else
  {
    Serial.println("UNKNOWN");
  }
  uint64_t cardSize = SD.cardSize() / (1024 * 1024);
  Serial.printf("SD Card Size: %lluMB\n", cardSize);
}

void setup()
{

  // Start the Serial Monitor
  Serial.begin(115200);

  preferences.begin("my-app", false);

  Wire.begin();
  if (!rtc.begin())
  {
    Serial.println("Não foi possível encontrar RTC");
  }
  else
  {
    bool data_modulo = preferences.getBool("data_modulo", false);
    if (data_modulo == false)
    {

      rtc.adjust(DateTime(__DATE__, __TIME__));
      Serial.println("Data e hora definidas!");
      data_modulo = true;
    }
    preferences.putBool("data_modulo", data_modulo);
    preferences.end();
  }
  initSDCard();

  // Start the DS18B20 sensor
  sensors.begin();
}

void loop()
{
   data_hora_now();
  // sensors.requestTemperatures();
  // float temperatureC = sensors.getTempCByIndex(0);
  // float temperatureF = sensors.getTempFByIndex(0);
  // Serial.print(temperatureC);
  // Serial.println("ºC");
  // // Serial.print(temperatureF);
  // // Serial.println("ºF");
  // int value = analogRead(AOUT_PIN); // read the analog value from sensor

  // Serial.print("Moisture value: ");
  // Serial.println(value);

   delay(5000);
}