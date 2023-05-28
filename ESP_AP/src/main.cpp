#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include "SPIFFS.h"

WebServer server(80);

const char *ssid = "ESP-DASHBOARD";
const char *password = "123456789";

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);



// Manipulador para páginas não encontradas
void handleNotFound()
{
  server.send(404, "text/plain", "Página não encontrada");
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
// Manipulador para a página principal
void handleRoot()
{
  loadFromSPIFFS("/index.html","text/html");
}
void handleRootcss()
{
  loadFromSPIFFS("/style.css","text/css");
}

void handleRootjs()
{
  loadFromSPIFFS("/script.js","text/js");
}
void setup()
{

  Serial.begin(115200);
  Serial.println();
  Serial.println("Configuring access point...");

  if (!WiFi.softAPConfig(local_ip, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
  }
  delay(1);
  if (!WiFi.softAP(ssid, password))
  {
    log_e("Soft AP creation failed.");
    while (1)
      ;
  }

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  server.begin();

  // Inicia o SPIFFS
  initSPIFFS();
  // Adiciona a função "handle_on_connect" quando o servidor estiver online

  server.on("/", handleRoot);
  server.on("/style.css", handleRootcss);
  server.on("/script.js", handleRootjs);

  // Adiciona a função "handle_not_found" quando o servidor estiver offline
  server.onNotFound(handleNotFound);

  // Route to load style.css and script.js file
  // Inicia o servidor
  server.begin();

  Serial.println("Server started");
}

void loop(void)
{
  server.handleClient();
  delay(5); // allow the cpu to switch to other tasks
}