#include <WiFi.h>                                                                 //Inclusão das bibliotecas
#include <WebServer.h>
#define trig 25
#define echo  26

float trigPuls();
float media_pulse();
float dist_cma = 0;
WebServer sv(80);
const char* ssid= "Selma Maria_Fibra";                                             //Dados da sua rede Wi-Fi                                                
const char* senha = "88599380";

void conectado() {                                                                //Sub-rotina para caso o servidor fique online
  sv.send(200, "text/html", html(dist_cma));     //Envia ao servidor, em formato HTML, o nosso script, com os parâmetros de pressão e temperatura
}

void nao_encontrado() {                                                           //Sub-rotina para caso seja retornado um erro
  sv.send(404, "text/plain", "Não encontrado");                                   //Retorna a mensagem de erro em caso de um retorno 404
}

String html(float dist) {
  String ptr = "<!DOCTYPE html>\n";
  ptr += "<html lang=\"pt-br\">\n";
   ptr += "<head><meta http-equiv=\"refresh\" content=\"0.5\"><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<meta charset=\"UTF-8\">\n";
  ptr += "<meta http-equiv=\"X-UA-Compatible\" content=\"IE=edge\">\n";
  ptr += "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += " <title>Controle do Reservatório</title>  \n";
  ptr += " <style>\n";
  ptr += "html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;}\n";
  ptr += " .icons{float:left; width:40px; position: relative;}\n";
  ptr += ".dados{\n";
  ptr += "border: solid;\n";
 ptr += " border-image-slice: 1;\n";
 ptr += "border-width: 5px;\n";
 ptr += "border-image-source: linear-gradient(60deg,#b16ec4,#5f97b9);\n";
 ptr += "contain: content;background-color: aqua; margin-top: 50px;}\n";
 ptr += "h1 {color: #444444; margin: 50px auto 30px;}\n";
 ptr += "p {font-size: 24px; color: #444444; margin-bottom: 10px;}\n";
 ptr += ".linha_horizontal{\n";
 ptr += " width: 100%;\n";
 ptr += " border: 1px solid #000;}\n";
 ptr += "</style>\n";
 ptr += " </head>\n";
 ptr += "<header>\n";
 ptr += "<div class=\"linha_horizontal\"></div>\n";
 ptr += " <h1> Medidor de Nivel da Caixa D'agua </h1>\n";
 ptr += "<img src=\"https://encrypted-tbn0.gstatic.com/images?q=tbn:ANd9GcQLn4QU9JxNcTbbu7VRcnXjrDbIQCCaMTqN8g&amp;usqp=CAU\">\n";
 ptr += "<h5>Com Sensor Ultrasônico JSN-SR04T e Esp32</h5>\n";
 ptr += " <div class=\"linha_horizontal\"></div>\n";
 ptr += " </header>\n";
 ptr += " <body>\n";
 ptr += "<section>\n";
 ptr += " <div class=\"dados\" id=\"webpage\">\n";
 ptr += "<h1>Dados em tempo real</h1>\n";
 ptr += "<img class=\"icons\" src=\"data:image/png;base64,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\">\n";
 ptr += "<p><h2> NÍVEL DA ÁGUA: </h2></p>\n";
 ptr += " <img class=\"icons\" src=\"https://media.istockphoto.com/vectors/thin-line-icon-laundry-liquid-water-volume-measurement-vector-id1129106120?k=20&m=1129106120&s=170667a&w=0&h=BeGT9bFPkajXkJ72VSjoTQFAsfjs_50-oqfDF9PoOrg=\">\n";
 ptr += (float) dist ;
 ptr += "<p><h2>  VOLUME ATUAL:</h2></p>\n";
 ptr += " <img class=\"icons\" src=\"https://www.pikpng.com/pngl/m/349-3494712_pump-icon-free-png-and-svg-download-centrifugal.png\">\n";
 ptr += " <p><h2>  STATUS DA BOMBA:</h2></p>\n";
 ptr += " </div>  \n";
 ptr += " </section>\n";
 ptr += " </body>\n";
 ptr += "<footer>\n";
 ptr += "<div class=\"linha_horizontal\"></div>\n";
 ptr += " <p><h4>Feito por : Aline Mariana</h4></p>\n";
 ptr += "<p><h4>2022</h4></p>\n";
 ptr += " </footer>\n";
 ptr += "</html>\n";
                 
  return ptr;
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

void setup() {
  
  Serial.begin(115200);                                                           //Inicia o monitor serial
  delay(100);
  
  Serial.print("Se conectando a: ");
  Serial.println(ssid);
  WiFi.begin(ssid, senha);                                                        //Se conecta ao Wi-Fi

  while (WiFi.status() != WL_CONNECTED) {                                         //Verifica se a conexão foi bem-sucedida
    delay(1000);
    Serial.print(".");
    
  }

  Serial.println("\nConectado");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());                                                 //Imprime o endereço de IP

  sv.on("/", conectado);
  sv.onNotFound(nao_encontrado);
  sv.begin();                                                                     //Inicia o servidor

  Serial.println("Servidor Online");
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  digitalWrite(trig,LOW);

}

void loop() { 
  sv.handleClient(); 
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
     
}
