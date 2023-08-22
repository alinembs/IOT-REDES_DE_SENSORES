#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#define trig 25
#define echo  26

float trigPuls();
float media_pulse();
float dist_cma = 0;
// Replace with your network credentials
const char* ssid = "Selma Maria_Fibra"; 
const char* password = "88599380";

const char* PARAM_INPUT_1 = "state";

const int output = 25;
const int buttonPin = 26;

// Variables will change:
int ledState = LOW;          // the current state of the output pin
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="pt-br">
<head>
  <meta charset="UTF-8"/>
  <meta content="\" http-equiv="\" />
  <meta content="\" initial-scale="1.0," name="\" user-scalable="no\" width="device-width,&quot;" />
  <title>MEDIDOR NIVEL E AUTOMATIZADOR DO RESERVAT&Oacute;RIO</title>
    
  <style type="text/css">html { font-family: 'Open Sans', sans-serif; display: block; margin: 0px auto; text-align: center;color: #333333;
  

}

body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
    .switch {position: relative; display: inline-block; width: 120px; height: 68px} 
    .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
    .slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
    input:checked+.slider {background-color: #2196F3}
    input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}

h1 {margin: 50px auto 30px;}
side-by-side{display: inline-block;vertical-align: middle;position: relative;}
.humidity-icon{background-color: #3498db;width: 30px;height: 30px;border-radius: 50%;line-height: 36px;}
.humidity-text{font-weight: 600;padding-left: 15px;font-size: 19px;width: 160px;text-align: left;}
.humidity{font-weight: 300;font-size: 60px;color: #3498db;}
.superscript{font-size: 17px;font-weight: 600;position: absolute;right: -20px;top: 15px;}
.data{padding: 10px;}

  </style>
<link rel="stylesheet" href="estilos.css"/>


<script language="javascript">

<script>function toggleCheckbox(element) {
  var xhr = new XMLHttpRequest();
  if(element.checked){ xhr.open("GET", "/update?state=1", true); }
  else { xhr.open("GET", "/update?state=0", true); }
  xhr.send();
}

setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var inputChecked;
      var outputStateM;
      if( this.responseText == 1){ 
        inputChecked = true;
        outputStateM = "On";
      }
      else { 
        inputChecked = false;
        outputStateM = "Off";
      }
      document.getElementById("output").checked = inputChecked;
      document.getElementById("outputState").innerHTML = outputStateM;
    }
  };
  xhttp.open("GET", "/state", true);
  xhttp.send();
}, 1000 ) ;



</script>


</head>
<body>
  <div>
    <h6> <br>Universidade Estadual do Maranhão <br>
    Engenharia da Computação<br> Instrumentação Eletrônica </h6>

  </div>
<div id="\">
<header id="cabecalho">
<hr />
<h1>MEDIDOR DE N&Iacute;VEL<br />
E<br />
AUTOMATIZADOR DA CAIXA D&#39;&Aacute;GUA</h1>
<img alt="" src="https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQLn4QU9JxNcTbbu7VRcnXjrDbIQCCaMTqN8g&amp;usqp=CAU" style="width: 150px; height: 150px;" />
<h6>Com Sensor Ultrasônico JSN-SR04T e Esp32 </h6>
<hr />
</header>
</div>
<section id="caixa">
<div  id="informacao">
<h5><b>Nivel da Água: 0 %</b></h5>
<h5><b>Distancia em Cm: 0.0 </b></h5>
<h5><b>Status da Bomba: 
<h4><span id="outputState"></span></h4>
<label class="switch">
<input type="checkbox" onchange="toggleCheckbox(this)" id="output">
<span class="slider"></span></label>
 </b></h5>
</div>
</section>
<div class="\">
<div 0="" 13="" 1999="" 2000="" class="\" enable-background="\" height="\" http:="" id="\" svg="" version="\" viewbox="\" width="\" www.w3.org="" x="\" xml:space="\" xmlns="\" xmlns:xlink="\" y="\">
<hr /></div>
</div>
</body>

<h5>Feito por Aline Mariana </h5>
</html>

)rawliteral";

// Replaces placeholder with button section in your web page
String processor(const String& var, float dist ){
  //Serial.println(var);
  if(var == "BUTTONPLACEHOLDER"){
    String buttons ="";
    String outputStateValue = outputState();
    buttons+= "<h4>Output - GPIO 2 - State <span id=\"outputState\"></span></h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"toggleCheckbox(this)\" id=\"output\" " + outputStateValue + "><span class=\"slider\"></span></label>";
    return buttons;
  }
  return String();
}

String outputState(){
  if(digitalRead(output)){
    return "checked";
  }
  else {
    return "";
  }
  return "";
}
float trigPulse()
{
float pulse;
float dist_cm;
float erro = 3.5 ;
float dist_calibrada = 0;
digitalWrite(trig, HIGH);
delayMicroseconds(10);
digitalWrite(trig,LOW); 
 pulse = pulseIn(echo, HIGH, 200000);
 dist_cm = pulse/58.82;
 dist_calibrada = dist_cm + erro;

  return dist_calibrada;
}

float media_pulse()
{
 float leituraSum = 0;
 float resultado = 0 ;

for (int index = 0 ; index< 30; index++ ){

  delay(30);
  leituraSum +=trigPulse();

} 
  resultado = (float) leituraSum/30;
return resultado;
  
}
void setup(){
  // Serial port for debugging purposes
  Serial.begin(115200);

  pinMode(output, OUTPUT);
  digitalWrite(output, LOW);
  pinMode(buttonPin, INPUT);
  
  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  // Print ESP Local IP Address
  Serial.println(WiFi.localIP());

  // Route for root / web page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  // Send a GET request to <ESP_IP>/update?state=<inputMessage>
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // GET input1 value on <ESP_IP>/update?state=<inputMessage>
    if (request->hasParam(PARAM_INPUT_1)) {
      inputMessage = request->getParam(PARAM_INPUT_1)->value();
      inputParam = PARAM_INPUT_1;
      digitalWrite(output, inputMessage.toInt());
      ledState = !ledState;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }
    Serial.println(inputMessage);
    request->send(200, "text/plain", "OK");
  });

  // Send a GET request to <ESP_IP>/state
  server.on("/state", HTTP_GET, [] (AsyncWebServerRequest *request) {
    request->send(200, "text/plain", String(digitalRead(output)).c_str());
  });
  // Start server
  server.begin();

   pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  digitalWrite(trig,LOW);

}
  
void loop() {


  dist_cma = media_pulse();

  if (dist_cma <= 19){
    
    Serial.println("Out of range");
  }
  else {
    
    Serial.print(dist_cma, 0);
    Serial.print("cm,  ");
    Serial.println();
  }
  delay(1000);  
  // read the state of the switch into a local variable:
  int reading = digitalRead(buttonPin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH), and you've waited long enough
  // since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == HIGH) {
        ledState = !ledState;
      }
    }
  }

  // set the LED:
  digitalWrite(output, ledState);

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}
