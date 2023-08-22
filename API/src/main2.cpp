
#include <WiFi.h>
#include <WebServer.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Selma Maria_Fibra";
const char* password = "88599380";
const char* apiKey = "API_TESTE";
const char* apiEndpoint = "http://192.168.100.46:5000/inserir ";

WebServer server(80);

void handleRoot() {
  String html = "<html><body>";
  html += "<h1>Envio de dados para a API</h1>";
  html += "<form action=\"/enviar\" method=\"post\">";
  html += "Nome: <input type=\"text\" name=\"nome\"><br>";
  html += "Idade: <input type=\"text\" name=\"idade\"><br>";
  html += "<input type=\"submit\" value=\"Enviar\">";
  html += "</form>";
  html += "</body></html>";

  server.send(200, "text/html", html);
}

void handleForm() {
  if (server.method() == HTTP_POST) {
    String nome = server.arg("nome");
    String idade = server.arg("idade");

    DynamicJsonDocument jsonDoc(256);
    jsonDoc["nome"] = nome;
    jsonDoc["idade"] = idade;

    String requestBody;
    serializeJson(jsonDoc, requestBody);
    
    HTTPClient http;
    http.begin(apiEndpoint);
    http.addHeader("Content-Type", "application/json");
    http.addHeader("X-Api-Key", apiKey);


    int httpResponseCode = http.POST(requestBody);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(response);
      server.send(200, "text/html", "<h1>Dados enviados com sucesso para a API!</h1>");
    } else {
      Serial.printf("Erro na solicitação HTTP: %s\n", http.errorToString(httpResponseCode).c_str());
      server.send(500, "text/html", "<h1>Erro na solicitação HTTP</h1>");
    }

    http.end();
  } else {
    server.send(405, "text/html", "<h1>Método não permitido</h1>");
  }
}




void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando ao WiFi...");
  }

  Serial.println("Conectado ao WiFi");
  IPAddress myIP = WiFi.localIP();
  Serial.print("IP address: ");
  Serial.println(myIP);
  server.on("/", HTTP_GET, handleRoot);
  server.on("/enviar", HTTP_POST, handleForm);
  server.begin();
}

void loop() {
  server.handleClient();
  delay(10);
}

