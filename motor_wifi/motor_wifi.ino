#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>


const char* ssid = "Selma Maria_Fibra"; 
const char* password = "88599380";


String slider_value = "0";
String dir = "0";


int CHA  = 0;
int ENA   = 14; // this pin must be PWM enabled pin if Arduino board is used
int IN1  = 27;
int IN2  = 26;

// motor 2 settings
int IN3 = 25;
int IN4 = 33;
int ENB = 32;// this pin must be PWM enabled pin if Arduino board is used
int CHB = 1;

int  VELOCIDADE  =  0;
int   tipo =  -1;


const int frequency = 500;
const int resolution = 8;

const char* input_parameter = "value";
const char* input_parameter1 = "direcao";

AsyncWebServer server(80);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>DC Motor Speed Control Web Server</title>
  <style>
    html {font-family: Times New Roman; display: inline-block; text-align: center;}
    h2 {font-size: 2.3rem;}
    p {font-size: 2.0rem;}
    body {max-width: 400px; margin:0px auto; padding-bottom: 25px;}
    .slider { -webkit-appearance: none; margin: 14px; width: 360px; height: 25px; background: #4000ff;
      outline: none; -webkit-transition: .2s; transition: opacity .2s;}
    .slider::-webkit-slider-thumb {-webkit-appearance: none; appearance: none; width: 35px; height: 35px; background:#01070a; cursor: pointer;}
    .slider::-moz-range-thumb { width: 35px; height: 35px; background: #01070a; cursor: pointer; } 
  </style>
</head>
<body>
  <h2>Car-Robot- ESP32- Control Web Server</h2>
  <p><span id="textslider_value">%SLIDERVALUE%</span></p>
  <p><input type="range" onchange="updateSliderPWM(this)" id="pwmSlider" min="0" max="255" value="%SLIDERVALUE%" step="1" class="slider"></p>
  <p><span id = "direcao">%DIRECTION%</span></p>
   <div>
 <button onclick="Event1()">
    <img src="https://icons-for-free.com/iconfiles/png/512/up-131964753349128460.png"  style="max-widht:32px; max-height:32px;">
  </button>
</div>
  <div>
   
     <button onclick="Event3()">
    <img src="https://icons-for-free.com/iconfiles/png/512/left-131964752938970359.png"  style="max-widht:32px; max-height:32px;">
  </button>
       <button onclick="Event5()">  
     <img src="https://icons-for-free.com/iconfiles/png/512/wifi+48px-131987949053963278.png"  style="max-widht:32px; max-height:32px;">
        </button>
    <button onclick="Event4()">
    <img src="https://icons-for-free.com/iconfiles/png/512/right-131964753200680068.png"  style="max-widht:32px; max-height:32px;">
  </button>
    
  </div>
 <div>
 <button onclick="Event2()">
    <img src="https://icons-for-free.com/iconfiles/png/512/down-131964752665990361.png"  style="max-widht:32px; max-height:32px;">
  </button>
</div>
<script>
function updateSliderPWM(element) {
  var slider_value = document.getElementById("pwmSlider").value;
  document.getElementById("textslider_value").innerHTML = slider_value;
  console.log(slider_value);
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/slider?value="+slider_value, true);
  xhr.send();
}
function Event1(){
 var dir = 1 ;
 document.getElementById("direcao").innerHTML ="Frente";
 var xhr = new XMLHttpRequest();
 xhr.open("GET", "/slider?direcao="+dir, true);
 xhr.send();
} 
function Event2(){
 var dir = 2 ; 
 document.getElementById("direcao").innerHTML ="Voltar";
 var xhr = new XMLHttpRequest();
 xhr.open("GET", "/slider?direcao="+dir, true);
 xhr.send();
}  
function Event3(){
 var dir = 3 ;
 document.getElementById("direcao").innerHTML ="Esquerda";
 var xhr = new XMLHttpRequest();
 xhr.open("GET", "/slider?direcao="+dir, true);
 xhr.send();
}  
function Event4(){
 var dir = 4;  
 document.getElementById("direcao").innerHTML ="Direita";
 var xhr = new XMLHttpRequest();
 xhr.open("GET", "/slider?direcao="+dir, true);
 xhr.send();
}        
function Event5(){
 var dir = 0; 
 document.getElementById("direcao").innerHTML ="Parar";
 var xhr = new XMLHttpRequest();
 xhr.open("GET", "/slider?direcao="+dir, true);
 xhr.send();
} 

</script>
</body>
</html>
)rawliteral";



String processor(const String& var){
  if (var == "SLIDERVALUE"){
    return slider_value;
  }
  if (var == "DIRECTION"){
    return dir;
  }  
  
  return String();
}

void inicializar_motor(int ENA_pin, int IN1,int IN2, int pwm_channel, int frequency,int resolution)
{

  pinMode(ENA_pin, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  ledcSetup(pwm_channel, frequency, resolution);
  ledcAttachPin(ENA_pin, pwm_channel);
  
}
void Para_Frente(int IN1,int IN2,int IN3,int IN4,int CH1,int CH2,int VELOCIDADE )
{
    //VELOCIDADE
    ledcWrite(CH1, VELOCIDADE); 
    ledcWrite(CH2, VELOCIDADE);  
      
      
     //MOTOR  1
        digitalWrite(IN2, HIGH);
        digitalWrite(IN1, LOW);  
     //MOTOR 2

        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);
  
}

void Stop(int IN1,int IN2,int IN3,int IN4)
{
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

}

void Para_Tras(int IN1,int IN2,int IN3,int IN4,int CH1,int CH2,int VELOCIDADE )
{

      //VELOCIDADE
      ledcWrite(CH1, VELOCIDADE); 
      ledcWrite(CH2, VELOCIDADE); 
      
  
     //MOTOR  1
        digitalWrite(IN2, LOW);
        digitalWrite(IN1, HIGH);  
     //MOTOR 2

        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
  
}
void Para_Esq(int IN1,int IN2,int IN3,int IN4,int CH1,int CH2,int VELOCIDADE ){

   //VELOCIDADE
      ledcWrite(CH1, VELOCIDADE); 
      ledcWrite(CH2, VELOCIDADE); 
      
  
     //MOTOR  1
        digitalWrite(IN2, HIGH);
        digitalWrite(IN1, LOW);  
     //MOTOR 2

        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);



  
}

void Para_Dir(int IN1,int IN2,int IN3,int IN4,int CH1,int CH2,int VELOCIDADE)
{

   //VELOCIDADE
      ledcWrite(CH1, VELOCIDADE); 
      ledcWrite(CH2, VELOCIDADE); 
      
  
     //MOTOR  1
        digitalWrite(IN1, LOW);
        digitalWrite(IN2, LOW);  
     //MOTOR 2

        digitalWrite(IN3, HIGH);
        digitalWrite(IN4, LOW);

  
}


void setup(){

  Serial.begin(115200);
  delay(1000);
  
  inicializar_motor(ENA, IN1,IN2, CHA, frequency, resolution);
  delay(500);
  inicializar_motor(ENB, IN3,IN4, CHB,  frequency, resolution);
  delay(500);
  

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html, processor);
  });

  server.on("/slider", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String message;
    if (request->hasParam(input_parameter)) {
      
      message = request->getParam(input_parameter)->value();
      slider_value = message;
      VELOCIDADE  =   slider_value.toInt();
      Serial.println("p1");

      
    }
    if (request->hasParam(input_parameter1)) {
      message = request->getParam(input_parameter1)->value();
      dir = message;
      tipo  = dir.toInt();
      Serial.println("p2");
      
      
    }
   
    else {
      message = "No message sent";
    }    
    Serial.println(message);
    

    request->send(200, "text/plain", "OK");
  });
  
  server.begin();
}
  
void loop() {

delay(1000);

//Frente
if (tipo  == 1)
{
  Para_Frente( IN1, IN2, IN3, IN4, CHA, CHB,VELOCIDADE);
  delay(100);
  Serial.println("FRENTE");
  
}
//VOLTAR

if (tipo == 2)
{
 Para_Tras( IN1, IN2, IN3, IN4, CHA, CHB,VELOCIDADE);
 delay(100);
  Serial.println("TRAS");
} 
//ESQUERDA

if (tipo == 3)
{
 Para_Esq( IN1, IN2, IN3, IN4, CHA, CHB,VELOCIDADE);
  delay(100);
  Serial.println("ESQ");
} 

//DIREITA
if (tipo == 4)
{
 Para_Dir( IN1, IN2, IN3, IN4, CHA, CHB,VELOCIDADE);
  delay(100);
  Serial.println("DIR");
} 

//STOP
if(tipo  ==  0)
{
Stop(IN1,IN2,IN3,IN4);
  delay(100);
  Serial.println("STOP");
}

else
{

 Serial.println("NDA"); 
 delay(100);
 
}


  
}


