#include <Arduino.h>
/*********
  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/
#include <ESPAsyncWebServer.h>
#include <WiFi.h>
#include <ESP32Servo.h>

Servo myservo;
Servo servo2;

 // create servo object to control a servo
// twelve servo objects can be created on most boards

// GPIO the servo is attached to
static const int servoPin = 26;
static const int servoPin1 = 27;

// Replace with your network credentials

const char *ssid = "ESP-DASHBOARD";
const char *password = "123456789";
// const char *ssid = "WifiEsp";
// const char *password = "88599380";
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
        xhr.open("GET", "/slider?value_base="+pos, true);
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
        xhr.open("GET", "/slider?value_hori="+pos, true);
        xhr.send();
     
    }
     
  </script>
</body>
</html>

)rawliteral";

void setup()
{
    Serial.begin(115200);
    // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
	myservo.setPeriodHertz(50);    // standard 50 hz servo
	myservo.attach(servoPin, 1000, 2000);
    //myservo.attach(servoPin); // attaches the servo on the servoPin to the servo object
    servo2.setPeriodHertz(50);

    servo2.attach(servoPin1,1000,2000);

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
   
    if (request->hasParam("value_base")) {
      
      message = request->getParam("value_base")->value();
      Serial.println("Position Servo 1:"+message);
    myservo.write(message.toInt());
    }
    if ((request->hasParam("value_hori"))) {
      message1 = request->getParam("value_hori")->value();
  Serial.println("Position Servo 2:"+message1);
    servo2.write(message1.toInt());}
    else {
      message = "No message sent";
    }    
    
  
    request->send(200, "text/plain", "OK"); });

    server.begin(); 
    
     }

void loop(){


}