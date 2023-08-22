#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "Selma Maria_Fibra";
const char* password = "88599380";
const char* apiKey = "API_TESTE";
const char* apiUrl = "http://192.168.100.46:5000/inserir ";

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi");
}

void solicitacao()
{
HTTPClient http;
 if (WiFi.status() == WL_CONNECTED) {
    // Configura o cabeçalho da requisição
    http.begin(apiUrl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-API-Key", apiKey);

    // Cria o corpo da requisição (para o método POST)
    String requestBody = "{\"nome\":\"Anna\",\"idade\":30}";

    // Envia a solicitação POST para inserir um novo registro
    int httpResponseCode = http.POST(requestBody);

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
}
void loop()
{

  solicitacao();
  delay(30000); // Aguarda 5 segundos antes de fazer a próxima solicitação
}
/*void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    // Configura o cabeçalho da requisição
    http.begin(apiUrl);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-API-Key", apiKey);

    // Cria o corpo da requisição (para o método POST)
    String requestBody = "{\"nome\":\"Exemplo\",\"idade\":25}";

    // Envia a solicitação POST para inserir um novo registro
    int httpResponseCode = http.POST(requestBody);

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

  delay(5000); // Aguarda 5 segundos antes de fazer a próxima solicitação
}
*/