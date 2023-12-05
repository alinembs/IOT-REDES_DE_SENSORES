
#include <ArduinoJson.h>
#include <HTTPClient.h>
#include "config_data_hora_sd.h"
#include "config_humidade.h"
#include "config_temperatura.h"


const char *apiEndpoint = "http://raspiberrypi-pv.local:5000/dados-do-sensor ";
const char *apiKey = "API_TESTE";


// MANIPULADOR PARA ENVIAR OS DADOS PARA A APLICAÇÃO WEB E SALVAR NA WEB
void sendSensorData()
{
    DynamicJsonDocument jsonDoc(256);
    // jsonDoc["data_hora"] = data_now();
    jsonDoc["temperatura_c"] = readDSTemperatureC();
    jsonDoc["temperatura_f"] = readDSTemperatureF();
    jsonDoc["umidade"] = onSensorChange();
    jsonDoc["vazao"] = 0;
    String requestBody;
    serializeJson(jsonDoc, requestBody);

    HTTPClient http;
    http.begin(apiEndpoint);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", apiKey);

    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0)
    {
      String response = http.getString();
      Serial.println(response);
      server.send(200, "text/html", "<h1>Dados enviados com sucesso para a API!</h1>");
    }
    else
    {
      Serial.printf("Erro na solicitação HTTP: %s\n", http.errorToString(httpResponseCode).c_str());
      server.send(500, "text/html", "<h1>Erro na solicitação HTTP</h1>");
    }

    http.end();
}
