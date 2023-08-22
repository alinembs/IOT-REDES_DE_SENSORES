// INCLUDE DOS CONFIG

#include <WiFi.h>
#include <WiFiClient.h>
#include <HTTPClient.h>


const char *ssid = "Selma Maria_Fibra";
const char *password = "88599380";


/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

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


// VARIAVEIS PARA SE COMUNICAR COM API DO BANCO DE DADOS
const char *apiKey = "API_TESTE";
const char *apiEndpoint = "http://192.168.100.63:5000/verificar-conexao";

// MANIPULADOPR PARA ENVIAR OS DADOS PARA BASE DE DADOS
void handle_conect_api()
{
 
    HTTPClient http;
    http.begin(apiEndpoint);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", apiKey);

    int httpResponseCode = http.GET();

    if (httpResponseCode > 0)
    {
      String response = http.getString();
      Serial.println("Respoda da API");
      Serial.println(response);
    
    }
    else
    {
      Serial.printf("Erro na solicitação HTTP: %s\n", http.errorToString(httpResponseCode).c_str());
     
    }

    http.end();
  }
 
void setup() {
 Serial.begin(115200);
 init_Wifi_NM();
}

void loop() {
Serial.println("-  Loop -");
handle_conect_api();
delay(10000);
}

