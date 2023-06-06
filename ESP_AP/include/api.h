#include "config_wifi.h"
#include <HTTPClient.h>

const char* apiEndpoint = "http://127.0.0.1:5000/adicionar-dados";

const char* apiKey = "AaaasWsugfd6jjKKWyaSs8s_7SS8222";

//WiFiClient http;
// Cria o objeto HTTPClient
  HTTPClient http;


void connectar_api(String dataHora, String humidity, String temperaturaF, String temperaturaC)
{


  // Configura a URL da requisição
  http.begin(apiEndpoint);

  // Define o cabeçalho da requisição com a chave de API
  http.addHeader("X-API-Key", apiKey);

  // Cria o corpo da requisição com os dados dos sensores
  String requestBody = "{\"temperatura_c\":" + temperaturaC + ",";
  requestBody += "\"temperatura_f\":" + temperaturaF + ",";
  requestBody += "\"umidade\":" + humidity + ",";
  requestBody += "\"data_hora\":\"" + dataHora + "\"}";

  // Envia a requisição POST com o corpo dos dados dos sensores
  int httpResponseCode = http.POST(requestBody);

  // Verifica o código de resposta da requisição
  if (httpResponseCode == 200) {
    Serial.println("Dados enviados com sucesso!");
  } else {
    Serial.print("Falha no envio dos dados. Código de resposta: ");
    Serial.println(httpResponseCode);
  }

  // Libera os recursos
  http.end();

}