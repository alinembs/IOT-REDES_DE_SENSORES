#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include <OneWire.h>
#include <DallasTemperature.h>


#include <Servo.h>
//
#define limit 1500
#define min_moisture 4095
#define max_moisture 2500

float Percent_Moisture(int value)
{
int valor = 100 - ((value - max_moisture)*100)/limit;
if(valor < -1)
{valor = 0;}
return valor;
}

//


#define AOUT_PIN 36 // ESP32 pin GIOP36 (ADC0) that connects to AOUT pin of moisture sensor


// GPIO where the DS18B20 is connected to
// Data wire is connected to GPIO 4
#define ONE_WIRE_BUS 4

// Setup a oneWire instance to communicate with any OneWire devices
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature sensor 
DallasTemperature sensors(&oneWire);

//
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position


// Variables to store temperature values
String temperatureC = "";

//
String humidity = " ";

//

String estado_servo = " ";

// Replace with your network credentials
const char* ssid = "Wif";
const char* password = "123456789";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

String readDSTemperatureC() {
  // Call sensors.requestTemperatures() to issue a global temperature and Requests to all devices on the bus
  sensors.requestTemperatures(); 
  float tempC = sensors.getTempCByIndex(0);

  if(tempC == -127.00) {
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  } else {
    Serial.print("Temperature Celsius: ");
    Serial.println(tempC); 
  }
  return String(tempC);
}

String readDSHumidity()
{
     int value = analogRead(AOUT_PIN); 
     float percent = Percent_Moisture(value);
   if((percent < -1) &(percent > 100 )) {
    Serial.println("Failed to read from Humidity sensor");
    return "--";
  } else {
    Serial.print("Humidity: ");
    //percent = 0;
    Serial.println(percent); 
  }
  return String(percent);  


}

String writeServo(int tipo) {
  String state = "";
 
   if(tipo == 1 )
   {
    state = "Braço Expandindo";
    Serial.println(state);
   }
   if(tipo  == 2)
   {
    state="Braço Retraindo";
   }
   if(tipo == 3)
   {
    state = "Parado";
   }
   if(state == ""){
    Serial.println("Failed to conect from servo");
    return "--";
  } else {
    Serial.print("Estado do Braço: ");
    Serial.println(state);
  }
  return state;
}

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.1/css/fontawesome.min.css" integrity="sha512-giQeaPns4lQTBMRpOOHsYnGw1tGVzbAIHUyHRgn7+6FmiEgGGjaG0T2LZJmAPMzRCl+Cug0ItQ2xDZpTmEc+CQ==" crossorigin="anonymous" referrerpolicy="no-referrer" />
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.1/css/brands.min.css" integrity="sha512-G/T7HQJXSeNV7mKMXeJKlYNJ0jrs8RsWzYG7rVACye+qrcUhEAYKYzaa+VFy6eFzM2+/JT1Q+eqBbZFSHmJQew==" crossorigin="anonymous" referrerpolicy="no-referrer" />
  <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.1/css/solid.min.css" integrity="sha512-6mc0R607di/biCutMUtU9K7NtNewiGQzrvWX4bWTeqmljZdJrwYvKJtnhgR+Ryvj+NRJ8+NnnCM/biGqMe/iRA==" crossorigin="anonymous" referrerpolicy="no-referrer" />
  
  <style>
    html {
     font-family: Arial;
     display: inline-block;
     margin: 0px auto;
     text-align: center;
    }
    h2 { font-size: 3.0rem; }
    p { font-size: 3.0rem; }
    .units { font-size: 1.2rem; }
    .ds-labels{
      font-size: 1.5rem;
      vertical-align:middle;
      padding-bottom: 15px;
    }
  </style>
</head>
<body>
  <h2>Monitoramento de Dados do Solo</h2> 
  <p>
    <i class="fa-solid fa-robot" style="color:#059e8a;"> </i>
    <span class="ds-labels">Estado do Braço: </span> 
    <span id="servo">%SERVO%</span>
  </p>
  <p>
    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i> 
    <span class="ds-labels">Temperature Celsius</span> 
    <span id="temperaturec">%TEMPERATUREC%</span>
    <sup class="units">&deg;C</sup>
  </p>
  <p>
    <i class="fa-solid fa-droplet" style="color:#059e8a;"> </i>
    <span class="ds-labels">Humidity</span> 
    <span id="umidade">%HUMIDITY%</span>
    <sup class="units">%</sup>
  </p>  
 
</body>
<script>
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("temperaturec").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/temperaturec", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("umidade").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/umidade", true);
  xhttp.send();
}, 10000) ;
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("servo").innerHTML = this.responseText;
    }
  };
  xhttp.open("GET", "/servo", true);
  xhttp.send();
}, 10000) ;
</script>
</html>

)rawliteral";

// Replaces placeholder with DS18B20 values
String processor(const String& var){
  //Serial.println(var);
  if(var == "TEMPERATUREC"){
    return temperatureC;
  }
  else if(var =="HUMIDITY"){
    return humidity;
  }
  else if(var == "SERVO")
  {
    return estado_servo;
  }
  return String();
}

void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Start up the DS18B20 library
  sensors.begin();

  temperatureC = readDSTemperatureC();
  // Humidity
  humidity = readDSHumidity();

  //Servo
  estado_servo = writeServo(3);


  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", temperatureC.c_str());
  });
  server.on("/umidade", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", humidity.c_str());
  });
  server.on("/servo", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/plain", estado_servo.c_str());
  });
  // Start server
  server.begin();

   myservo.attach(26);

}
 
void loop(){
   
    temperatureC = readDSTemperatureC();
    humidity = readDSHumidity();
    estado_servo = writeServo(3);
   
    int value = analogRead(AOUT_PIN); 
    float percent = Percent_Moisture(value);
     if(percent < 60 )
  {
    estado_servo = writeServo(1);} 
    delay(3000);

    /*int pos = 0;
//Armar o Braço
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
         }

//Ligar o Motor 
    int limite=0;
    myservo.write(pos);
    while(limite < 90)
  { 
   
    int value = analogRead(AOUT_PIN); 
    float percent = Percent_Moisture(value);

    //Serial.println("Irrigando...");
    //Serial.println("Percentual:");
    //Serial.println(percent,2);
    //Serial.print("Moisture value: ");
    //Serial.println(value);
    //delay(3000);
    if(percent  > 70)
    {
      limite = 91;
    }
  }
  //Desligar Motor
 //Serial.println("Finalizando a Irrigação");
    delay(3000);
     estado_servo = writeServo(2);
//Voltar o Braço
   for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  //myservo.write(0);
  //Serial.println("Irrigação Concluida");
  //Serial.println("-------------------------");
  estado_servo = writeServo(3);

  }*/
  }  
