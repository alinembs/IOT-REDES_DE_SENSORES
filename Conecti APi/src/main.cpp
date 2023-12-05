#include <WiFi.h>
#include <WiFiClient.h>


#include <HTTPClient.h>

const char* ssid = "ESP-DASHBOARD";
const char* password = "123456789";


const char* apiKey = "API_TESTE";
const char* apiUrl = "http://192.168.1.2:5000/verificar-conexao ";

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);



void init_Wifi_AP()
{
    WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }
  Serial.println("Conexão WiFi estabelecida!");
}


void setup() {
  Serial.begin(115200);
  delay(100);
  init_Wifi_AP();

}

void solicitacao()
{
HTTPClient http;
 
    // Configura o cabeçalho da requisição
    http.begin(apiUrl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-API-Key", apiKey);

    // Envia a solicitação GET para inserir um novo registro
    int httpResponseCode = http.GET();

    // Verifica o código de resposta da API
    if (httpResponseCode == HTTP_CODE_OK) {
      String responseBody = http.getString();
      Serial.println(responseBody);

      // Processa a resposta da API
      // ...
    } else {
      Serial.print("Erro na solicitação: ");
      Serial.println(httpResponseCode);
    }

    http.end();

}
void loop()
{
  solicitacao();
  delay(10000); // Aguarda 10 segundos antes de fazer a próxima solicitação
}
