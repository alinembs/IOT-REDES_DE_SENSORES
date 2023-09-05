#include <WiFi.h>
#include <WiFiClient.h>

const char *ssid = "ESP-DASHBOARD";
const char *password = "123456789";

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

//Iniciar Wifi no Modo Acess Point
void init_Wifi_AP()
{
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
}
//Iniciar Wifi no Modo Normal
void init_Wifi_NM()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conectado ao WiFi!");
  IPAddress myIP = WiFi.localIP();
    Serial.print("IP address: ");
    Serial.println(myIP);

}

