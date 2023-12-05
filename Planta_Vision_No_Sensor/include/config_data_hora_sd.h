
#include "config_server.h"

#include "SPIFFS.h"

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <Wire.h>
#include <RTClib.h>

#include <Preferences.h>
Preferences preferences;

RTC_DS3231 rtc;
String data_hora = "";


// Iniciar o Cartão de Memoria
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
// Inicia o SPIFFS
void initSPIFFS()
{
  if (!SPIFFS.begin())
  {
    Serial.println("An error has occurred while mounting SPIFFS");
  }
  else
  {
    Serial.println("SPIFFS mounted successfully");
  }

  // File file = SPIFFS.open("/index.html", "r");

  // file.close();
}
// Pegar arquivos do SD
bool loadFromSD(fs::FS &fs, String path, String dataType)
{
  Serial.print("Requested page -> ");
  Serial.println(path);
  if (fs.exists(path))
  {
    File dataFile = fs.open(path, "r");
    if (!dataFile)
    {
      handleNotFound();
      return false;
    }

    if (server.streamFile(dataFile, dataType) != dataFile.size())
    {
      Serial.println("Sent less data than expected!");
    }
    else
    {
      Serial.println("Page served!");
    }

    dataFile.close();
  }
  else
  {
    handleNotFound();
    return false;
  }
  return true;
}
// Pegar dados do SPIFFS
bool loadFromSPIFFS(String path, String dataType)
{
  Serial.print("Requested page -> ");
  Serial.println(path);
  if (SPIFFS.exists(path))
  {
    File dataFile = SPIFFS.open(path, "r");
    if (!dataFile)
    {
      handleNotFound();
      return false;
    }

    if (server.streamFile(dataFile, dataType) != dataFile.size())
    {
      Serial.println("Sent less data than expected!");
    }
    else
    {
      Serial.println("Page served!");
    }

    dataFile.close();
  }
  else
  {
    handleNotFound();
    return false;
  }
  return true;
}
// Possiblitar baixar arquivos que estão no SD para o Navegador
void SD_file_download(String filename)
{
  File download = SD.open("/" + filename);
  if (download)
  {
    server.sendHeader("Content-Type", "text/text");
    server.sendHeader("Content-Disposition", "attachment; filename=" + filename);
    server.sendHeader("Connection", "close");
    server.streamFile(download, "application/octet-stream");
    download.close();
  }
}
// Escrever dados em um arquivo dentro do SD/SPIFFS
void writeFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Writing file: %s\n", path);

  File file = fs.open(path, FILE_WRITE);
  if (!file)
  {
    Serial.println("Failed to open file for writing");
    return;
  }
  if (file.print(message))
  {
    Serial.println("File written");
  }
  else
  {
    Serial.println("Write failed");
  }
  file.close();
}
// Adicionar dados a arquivos já criados dentro do SD/SPIFFS
void appendFile(fs::FS &fs, const char *path, const char *message)
{
  Serial.printf("Appending to file: %s\n", path);

  File file = fs.open(path, FILE_APPEND);
  if (!file)
  {
    Serial.println("Failed to open file for appending");
    return;
  }
  if (file.println(message))
  {
    Serial.println("Message appended");
  }
  else
  {
    Serial.println("Append failed");
  }
  file.close();
}
// Inicializa o RTC e Ajusta a Data-Hora do Esp32 se for a primeira vez
void init_RTC()
{
  preferences.begin("my-app", false);
  Wire.begin();
  if (!rtc.begin())
  {
    Serial.println("Não foi possível encontrar RTC");
    //while(1);
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
}

// Pega a Data-Hora Atual e salva em uma Variavel Global
String data_now()
{
  DateTime now = rtc.now();
  // Formato da data: DD/MM/AAAA
  String data = String(now.year()) + "-" + String(now.month()) + "-" + String(now.day());
  //Serial.println(data);
  // Formato da hora: HH:MM:SS
  String hora = String(now.hour()) + ":" + String(now.minute()) + ":" + String(now.second());
  //Serial.println(hora);
  return String(data + ' ' + hora);
}