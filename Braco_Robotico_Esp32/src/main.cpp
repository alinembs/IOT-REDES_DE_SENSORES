#include <Arduino.h>
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <Servo.h>

Servo SERVO_HORIZONTAL;
Servo SERVO_VERTICAL;
Servo SERVO_BASE;

// GPIO the servo is attached to
static const int servoPin1 = 25;
static const int servoPin2 = 26;
static const int servoPin3 = 27;

// Replace with your network credentials

const char *ssid = "ESP-DASHBOARD";
const char *password = "123456789";
// Set web server port number to 80

AsyncWebServer server(80);
// Variable to store the HTTP request

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
  <p>Position Servo 1: <span id="servoPos"></span></p>
  <input type="range" min="0" max="180" class="slider" id="servoSlider" onchange="servo(this.value)"/>
  
   <p>Position Servo 2: <span id="servoPos1"></span></p>
  
  <input type="range" min="0" max="180" class="slider" id="servoSlider1" onchange="servo1(this.value)"/>

   <p>Position Servo 3: <span id="servoPos2"></span></p>
  <input type="range" min="0" max="180" class="slider" id="servoSlider2" onchange="servo2(this.value)"/>
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
    var slider1 = document.getElementById("servoSlider1");
    var servoP1 = document.getElementById("servoPos1");
    servoP1.innerHTML = slider1.value;
    slider1.oninput = function() {
      slider1.value = this.value;
      servoP1.innerHTML = this.value;
    }
   
    function servo1(pos) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/slider?value1="+pos, true);
        xhr.send();
     
    }
     
  var slider2 = document.getElementById("servoSlider2");
    var servoP2 = document.getElementById("servoPos2");
    servoP2.innerHTML = slider2.value;
    slider2.oninput = function() {
      slider2.value = this.value;
      servoP2.innerHTML = this.value;
    }
   
    function servo2(pos) {
        var xhr = new XMLHttpRequest();
        xhr.open("GET", "/slider?value2="+pos, true);
        xhr.send();
     
    }
  </script>
</body>
</html>

)rawliteral";

void setup()
{
  Serial.begin(115200);

  SERVO_HORIZONTAL.attach(servoPin1); // attaches the servo on the servoPin to the servo object
  SERVO_VERTICAL.attach(servoPin2);
  SERVO_BASE.attach(servoPin3);

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
    String message1;
   
    if (request->hasParam("value")) {
      
      message = request->getParam("value")->value();
      Serial.println("Position Servo 1:"+message);
      SERVO_HORIZONTAL.write(message.toInt());
    }
    if ((request->hasParam("value1"))) {
      message1 = request->getParam("value1")->value();
    Serial.println("Position Servo 2:"+message1);
    SERVO_VERTICAL.write(message1.toInt());}
    if ((request->hasParam("value2"))) {
      message1 = request->getParam("value2")->value();
    Serial.println("Position Servo 3:"+message1);
    SERVO_BASE.write(message1.toInt());}

    else {
      message = "No message sent";
    }    
    
  
    request->send(200, "text/plain", "OK"); });

  server.begin();
}

void loop()
{
}
