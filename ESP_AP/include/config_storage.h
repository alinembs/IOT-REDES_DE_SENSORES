#include "SPIFFS.h"

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <WebServer.h>
#include <ESPmDNS.h>


WebServer server(80);


#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

String formattedDate;
String dayStamp;
String timeStamp;

// Manipulador para páginas não encontradas
void handleNotFound()
{
  server.send(404, "text/plain", "Página não encontrada");
}

// Iniciar o Cartão de Memoria
void initSDCard()
{
  if (!SD.begin())
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

void init_Date_Time()
{
    timeClient.begin();
    timeClient.setTimeOffset(39600);
}

void getTimeStamp()
{
    while (!timeClient.update())
    {
        timeClient.forceUpdate();
    }
    formattedDate = timeClient.getFormattedDate();
    int splitT = formattedDate.indexOf("T");
    dayStamp = formattedDate.substring(0, splitT);
    timeStamp = formattedDate.substring(splitT + 1, formattedDate.length() - 1);
}
