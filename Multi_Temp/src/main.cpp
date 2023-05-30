#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_1 27

#define ONE_WIRE_2 4

#define ONE_WIRE_3 26

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire1(ONE_WIRE_1);
OneWire oneWire2(ONE_WIRE_2);
OneWire oneWire3(ONE_WIRE_3);
// Pass our oneWire reference to Dallas Temperature sensor

DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);
DallasTemperature sensors3(&oneWire3);

// Variables to store temperature values
String temperatureC1 = "";
String temperatureC2 = "";
String temperatureC3 = "";

// Replace with your network credentials
const char *ssid = "WifiEsp";
const char *password = "12345678";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDSTemperatureC1()
{
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors1.requestTemperatures();
  float tempC = sensors1.getTempCByIndex(0);

  if (tempC == -127.00)
  {
    Serial.println("Failed to read from DS18B20 sensor 2");
    return "--";
  }
  else
  {
    Serial.print("Temperature Celsius 2: ");
    Serial.println(tempC);
  }
  return String(tempC);
}

String readDSTemperatureC2()
{
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors2.requestTemperatures();
  float tempC = sensors2.getTempCByIndex(0);

  if (tempC == -127.00)
  {
    Serial.println("Failed to read from DS18B20 sensor 4");
    return "--";
  }
  else
  {
    Serial.print("Temperature Celsius 4: ");
    Serial.println(tempC);
  }
  return String(tempC);
}
String readDSTemperatureC3()
{
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors3.requestTemperatures();
  float tempC = sensors3.getTempCByIndex(0);

  if (tempC == -127.00)
  {
    Serial.println("Failed to read from DS18B20 sensor 15");
    return "--";
  }
  else
  {
    Serial.print("Temperature Celsius 15: ");
    Serial.println(tempC);
  }
  return String(tempC);
}

const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="pt">
    <head>
        <meta charset="UTF-8" />
        <meta http-equiv="X-UA-Compatible" content="IE=edge" />
        <meta name="viewport" content="width=device-width, initial-scale=1.0" />
        <link href="https://cdn.jsdelivr.net/npm/remixicon@2.5.0/fonts/remixicon.css" rel="stylesheet" />
        <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.1/css/fontawesome.min.css" integrity="sha512-giQeaPns4lQTBMRpOOHsYnGw1tGVzbAIHUyHRgn7+6FmiEgGGjaG0T2LZJmAPMzRCl+Cug0ItQ2xDZpTmEc+CQ==" crossorigin="anonymous" referrerpolicy="no-referrer" />
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.1/css/brands.min.css" integrity="sha512-G/T7HQJXSeNV7mKMXeJKlYNJ0jrs8RsWzYG7rVACye+qrcUhEAYKYzaa+VFy6eFzM2+/JT1Q+eqBbZFSHmJQew==" crossorigin="anonymous" referrerpolicy="no-referrer" />
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.1/css/solid.min.css" integrity="sha512-6mc0R607di/biCutMUtU9K7NtNewiGQzrvWX4bWTeqmljZdJrwYvKJtnhgR+Ryvj+NRJ8+NnnCM/biGqMe/iRA==" crossorigin="anonymous" referrerpolicy="no-referrer" />
    
		<title>Monitoramento de Dados</title>
    <style>

@import url('https://fonts.googleapis.com/css?family=Josefin+Sans:400,400i,600,600i');

html,body{
  margin:0;
  height:100%;
  font-family: sans-serif;
  padding: 0;
  box-sizing: border-box;
 
}
 </style>
    </head>
    <body style="display: flex; flex-direction: column; justify-content: space-between; height: 100vh">
 
  
                  <span id="temperaturec1">%TMPC1%</span>
                   <sup class="units">&deg;C</sup>
                   

                  <span id="temperaturec2">%TMPC2%</span>
                   <sup class="units">&deg;C</sup>
                   


                  <span id="temperaturec3">%TMPC3%</span>
                   <sup class="units">&deg;C</sup>
                   
</body>
<script>

        setInterval(function ( ) {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById("temperaturec1").innerHTML = this.responseText;
            }
          };
          xhttp.open("GET", "/temperaturec1", true);
          xhttp.send();
        }, 10000) ;
         setInterval(function ( ) {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById("temperaturec2").innerHTML = this.responseText;
            }
          };
          xhttp.open("GET", "/temperaturec2", true);
          xhttp.send();
        }, 10000) ;
         setInterval(function ( ) {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById("temperaturec3").innerHTML = this.responseText;
            }
          };
          xhttp.open("GET", "/temperaturec3", true);
          xhttp.send();
        }, 10000) ;
       
        
</script>
</html>


)rawliteral";

// Replaces placeholder with DS18B20 values
String processor(const String &var)
{
  // Serial.println(var);
  if (var == "TMPC1")
  {
    return temperatureC1;
  }
  if (var == "TMPC2")
  {
    return temperatureC2;
  }
  else if (var == "TMPC3")
  {
    return temperatureC3;
  }
  return String();
}

// Set your Gateway IP address
IPAddress local_IP(192, 168, 141, 31);
IPAddress gateway(192, 168, 141, 0);
IPAddress subnet(255, 255, 255, 0);

void setup()
{
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();

  // Start up the DS18B20 library
  sensors1.begin();
  sensors2.begin();
  sensors3.begin();

  temperatureC1 = readDSTemperatureC1();
  temperatureC2 = readDSTemperatureC2();
  temperatureC3 = readDSTemperatureC3();

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet))
  {
    Serial.println("STA Failed to configure");
  }
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/html", index_html); });
  server.on("/temperaturec1", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", temperatureC1.c_str()); });
  server.on("/temperaturec2", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", temperatureC2.c_str()); });
  server.on("/temperaturec3", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", temperatureC3.c_str()); });

  // Start server
  server.begin();
}

void loop()
{

  // Leitura Inicial
  Serial.println("----------------------------");
  temperatureC1 = readDSTemperatureC1();
  temperatureC2 = readDSTemperatureC2();
  temperatureC3 = readDSTemperatureC3();

  delay(500);
}