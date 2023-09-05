

// Variaveis para ter controle
String estado_servo = "PARADO";
String estado_bomba = "Desligado";
String BOMBA_STATUS = "OFF";

const char *PARAM_SERVO = "value";
const char *PARAM_BOMBA = "relay";


int POSITION_SERVO = 0;

// VARIAVEIS PARA SE COMUNICAR COM API
const char *apiKey = "API_TESTE";

const char *apiEndpoint = "http://192.168.83.30:5000/adicionar-dados ";
const char *apiEndpoint_1 = "http://192.168.1.4:5000/braco";
const char *apiEndpoint_2 = "http://192.168.1.4:5000/bomba";

// MANIPULADOPR PARA CONTROLAR O BRAÇO ROBOTICO
void handle_controle_braco()
{

  String message = "";

  if (server.arg(PARAM_SERVO) == "")
  { // Parameter not found

    message = "Argument not found";
  }
  else
  { // Parameter found

    message += server.arg(PARAM_SERVO); // Gets the value of the query parameter
    POSITION_SERVO = message.toInt();
  }

  DynamicJsonDocument jsonDoc(256);
  jsonDoc["index_servo"] = "Base";
  jsonDoc["position_servo"] = POSITION_SERVO;
  String requestBody;
  serializeJson(jsonDoc, requestBody);

  HTTPClient http;
  http.begin(apiEndpoint_1);
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

// MANIPULADOPR PARA CONTROLAR A BOMBA DE AGUA
void handle_controle_bomba()
{
  
     String message = "";

  if (server.arg(PARAM_BOMBA) == "")
  { // Parameter not found

    message = "Argument not found";
  }
  else
  { // Parameter found

    message += server.arg(PARAM_BOMBA); // Gets the value of the query parameter
  }

  //Bomba_Agua(message);
  server.send(200, "text / plain", "OK"); // Returns the HTTP response
    DynamicJsonDocument jsonDoc(256);
    jsonDoc["Estado_da_Bomba"] = BOMBA_STATUS;
    String requestBody;
    serializeJson(jsonDoc, requestBody);

    HTTPClient http;
    http.begin(apiEndpoint_2);
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

// MANIPULADOPR PARA SALVAR OS DAODS NA BASE DE DADOS MYSQL
void handle_conect_api()
{
  data_now();
  if (server.method() == HTTP_POST)
  {

    DynamicJsonDocument jsonDoc(256);
    jsonDoc["data_hora"] = data_hora;
    jsonDoc["temperatura_c"] = temperatureC;
    jsonDoc["temperatura_f"] = temperatureF;
    jsonDoc["umidade"] = humidity;
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
  else
  {
    server.send(405, "text/html", "<h1>Método não permitido</h1>");
  }
}