#include <Arduino.h>
/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <Servo.h>

Servo myservo; // create servo object to control a servo
// twelve servo objects can be created on most boards

// GPIO the servo is attached to
static const int servoPin = 26;

// Replace with your network credentials

const char *ssid = "Selma Maria_Fibra";
const char *password = "88599380";
// Set web server port number to 80

AsyncWebServer server(80);
// Variable to store the HTTP request
String header;

// Decode HTTP GET value
String valueString = String(5);
int pos1 = 0;
int pos2 = 0;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <style>
    body {
      text-align: center;
      font-family: "Trebuchet MS", Arial;
      margin-left:auto;
      margin-right:auto;
    }
    .slider {
      width: 300px;
    }
  </style>
  <script src="https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js"></script>
</head>
<body>
  <h1>ESP32 with Servo</h1>
  <p>Position: <span id="servoPos"></span></p>
  <input type="range" min="0" max="180" class="slider" id="servoSlider" onchange="servo(this.value)"/>
  <script>
    var slider = document.getElementById("servoSlider");
    var servoP = document.getElementById("servoPos");
    servoP.innerHTML = slider.value;
    slider.oninput = function() {
      slider.value = this.value;
      servoP.innerHTML = this.value;
    }
   
    function servo(pos) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/slider?value="+pos, true);
        xhr.send();
     
    }
     
  </script>
</body>
</html>

)rawliteral";

void setup()
{
    Serial.begin(115200);

    myservo.attach(servoPin); // attaches the servo on the servoPin to the servo object

    // Connect to Wi-Fi network with SSID and password
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    // Print local IP address and start web server
    Serial.println("");
    Serial.println("WiFi connected.");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
              { request->send_P(200, "text/html", index_html); });
    server.on("/slider", HTTP_GET, [](AsyncWebServerRequest *request)
            {
    String message;
   
   
    if (request->hasParam("value")) {
      
      message = request->getParam("value")->value();
    
    }
  
    else {
      message = "No message sent";
    }    
    Serial.println("Position Servo:"+message);
    myservo.write(message.toInt());
    
   /*posicao_anterior = posicao_proximo;
    posicao_proximo = 0;*/ 
  
    
    request->send(200, "text/plain", "OK"); });

    server.begin();
}

void loop(){


}