#include "SPIFFS.h"

#include "FS.h"
#include "SD.h"
#include "SPI.h"

#include <WebServer.h>
#include <ESPmDNS.h>


WebServer server(80);

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