// ALINE MARIANA BARROS SILVA
//  ALGORITMO PARA MONITORAMENTO DO SOLO

// BIBIOTECAS

#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>

#include <OneWire.h>
#include <DallasTemperature.h>

#include <Servo.h>

////PINs USADOS NO PROJETO E DEFINIÇÕES E LIMITES

#define rele 27

#define limit 1295
#define min_moisture 4095
#define max_moisture 2800

#define otimo 75
#define regular 60
#define pessimo 50

#define ServoPin 26

#define AOUT_PIN 36

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);

DallasTemperature sensors(&oneWire);

Servo myservo;

int pos = 0;
int posicao_manual = 0;
//

// informações da WEB
String temperatureC = "";
String temperatureF = "";
String humidity = " ";
String estado_servo = " ";
String estado_bomba = " ";
String solo = " ";
String valor_servo = " ";

// FUNÇOES

float Percent_Moisture(int value)
{
  int valor = 100 - ((value - max_moisture) * 100) / limit;
  if (valor < -1)
  {
    valor = 0;
  }
  if (valor > 100)
  {
    valor = 100;
  }
  return valor;
}

void Servo_Manual(int posicao, Servo servo)
{
  servo.write(posicao);
  delay(15);
}

void Bomba_Agua(int state)
{
  if (state == 0)
  {
    digitalWrite(rele, LOW);
  }
  else
  {
    digitalWrite(rele, HIGH);
  }
}

// CONFIG DO WEB SERVER

const char *ssid = "WifiEsp";
const char *password = "12345678";

AsyncWebServer server(80);

// FUNCOES DE LEITURAS

String readDSTemperatureC()
{

  sensors.requestTemperatures();
  float tempC = sensors.getTempCByIndex(0);

  if (tempC == -127.00)
  {
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  }
  else
  {
    Serial.print("Temperature Celsius: ");
    Serial.println(tempC);
  }
  return String(tempC);
}

String readDSTemperatureF()
{

  sensors.requestTemperatures();
  float tempF = sensors.getTempFByIndex(0);

  if (int(tempF) == -196)
  {
    Serial.println("Failed to read from DS18B20 sensor");
    return "--";
  }
  else
  {
    Serial.print("Temperature Fahrenheit: ");
    Serial.println(tempF);
  }
  return String(tempF);
}

String writeSolo(float percent)
{
  String solo = "";

  if (percent > 75)
  {
    solo = "Positivo";
    // Serial.println(solo);
  }
  if ((percent < 75) & (percent > 50))
  {
    solo = "Regular";
    // Serial.println(solo);
  }
  if (percent < 50)
  {
    solo = "Crítico";
    // Serial.println(solo);
  }

  if (solo == "")
  {
    Serial.println("Failed to find ground state");
    return "--";
  }
  else
  {
    Serial.print("Estado do Solo: ");
    Serial.println(solo);
  }
  return solo;
}

String readDSHumidity()
{
  int value = analogRead(AOUT_PIN);
  Serial.print("Humidity: ");
  Serial.println(value);
  float percent = Percent_Moisture(value);
  solo = writeSolo(percent);
  if ((percent < -1) & (percent > 100))
  {
    Serial.println("Failed to read from Humidity sensor");
    return "--";
  }
  else
  {
    Serial.print("Humidity %: ");
    // percent = 0;
    Serial.println(percent);
  }
  return String(percent);
}

String writeServo(int tipo)
{
  String state = "";

  if (tipo == 1)
  {
    state = "Expandindo";
    Serial.println(state);
  }
  if (tipo == 2)
  {
    state = "Retraindo";
  }
  if (tipo == 3)
  {
    state = "Parado";
  }
  if (state == "")
  {
    Serial.println("Failed to conect from servo");
    return "--";
  }
  else
  {
    Serial.print("Estado do Braço: ");
    Serial.println(state);
  }
  return state;
}
String writeBomba(int tipo)
{
  String state = "";

  if (tipo == 1)
  {
    state = "Ligado - ON";
    Serial.println(state);
  }
  if (tipo == 2)
  {
    state = "Desligando - OFF";
  }

  if (state == "")
  {
    Serial.println("Failed to conect from bomba");
    return "--";
  }
  else
  {
    Serial.print("Estado do Bomba: ");
    Serial.println(state);
  }
  return state;
}

const char index_html[] PROGMEM = R"rawliteral(

<!DOCTYPE html>
<html lang="pt">

<head>
    <meta charset="UTF-8" />
    <meta http-equiv="X-UA-Compatible" content="IE=edge" />
    <meta name="viewport" content="width=device-width, initial-scale=1.0" />
    <link href="https://cdn.jsdelivr.net/npm/remixicon@2.5.0/fonts/remixicon.css" rel="stylesheet" />
    <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css"
        integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.1/css/fontawesome.min.css"
        integrity="sha512-giQeaPns4lQTBMRpOOHsYnGw1tGVzbAIHUyHRgn7+6FmiEgGGjaG0T2LZJmAPMzRCl+Cug0ItQ2xDZpTmEc+CQ=="
        crossorigin="anonymous" referrerpolicy="no-referrer" />
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.1/css/brands.min.css"
        integrity="sha512-G/T7HQJXSeNV7mKMXeJKlYNJ0jrs8RsWzYG7rVACye+qrcUhEAYKYzaa+VFy6eFzM2+/JT1Q+eqBbZFSHmJQew=="
        crossorigin="anonymous" referrerpolicy="no-referrer" />
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.2.1/css/solid.min.css"
        integrity="sha512-6mc0R607di/biCutMUtU9K7NtNewiGQzrvWX4bWTeqmljZdJrwYvKJtnhgR+Ryvj+NRJ8+NnnCM/biGqMe/iRA=="
        crossorigin="anonymous" referrerpolicy="no-referrer" />
    <link rel="stylesheet" href="style.css">

    <title>Monitoramento de Dados</title>
    <style>

/* GERAL */


html,
body {
  display: flex;
  flex-direction: column;
  justify-content: space-between;
  height: 100%;

}

/*  HEADER */

@import url('https://fonts.googleapis.com/css?family=Josefin+Sans:400,400i,600,600i');

h4 {
  text-decoration: solid
}

.header {
  position: relative;
  overflow: hidden;
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  align-items: flex-start;
  align-content: flex-start;
  height: 50vw;
  min-height: 200px;
  max-height: 350px;
  min-width: 300px;
  color: #cfc0c0;
}

.header:after {
  content: "";
  width: 100%;
  height: 100%;
  position: absolute;
  bottom: 0;
  left: 0;
  z-index: -1;
  background: linear-gradient(to bottom, rgba(0, 0, 0, 0.12) 40%, rgba(27, 32, 48, 1) 100%);
}

.header:before {
  content: "";
  width: 100%;
  height: 200%;
  position: absolute;
  top: 0;
  left: 0;
  -webkit-backface-visibility: hidden;
  -webkit-transform: translateZ(0);
  backface-visibility: hidden;
  scale: (1.0, 1.0);
  transform: translateZ(0);
  background: #1B2030 url(https://images.unsplash.com/photo-1586771107445-d3ca888129ff?ixlib=rb-4.0.3&ixid=MnwxMjA3fDB8MHxzZWFyY2h8MTF8fGFncmljdWx0dXJlfGVufDB8fDB8fA%3D%3D&auto=format&fit=crop&w=1170&q=80) 50% 0 no-repeat;
  background-size: 100%;
  background-attachment: fixed;
  animation: grow 20s linear 10ms infinite;
  transition: all 0.4s ease-in-out;
  z-index: -2
}

.header a {
  color: #b4afaf;
}

.info {
  flex: 0 0 auto;
  width: 50%
}

.info {
  width: 100%;
  padding: 15% 10% 0% 0%;
  text-align: center;
  text-shadow: 0 2px 3px rgba(0, 0, 0, 0.2)
}

.info h4,
.meta {
  font-size: 0.7em
}

@keyframes grow {
  0% {
    transform: scale(1) translateY(0px)
  }

  50% {
    transform: scale(1.2) translateY(-400px)
  }
}


/* MENU  */

.tablink {
  background-color: #555;
  color: white;
  float: left;
  border: none;
  outline: none;
  cursor: pointer;
  padding: 14px 16px;
  font-size: 17px;
  width: 25%;
}

.tablink:hover {
  background-color: #777;
}

.menu {
  display: flex;
}

#corpo-dados {
  background-color: #0d0f27;
}

#config {
  background-color: rgb(9, 51, 9);
}

#atuadores {
  background-color: rgb(48, 17, 7);
}

#Add {
  background-color: rgb(46, 15, 43);
}


/*  MODO - 1 */
.modo-1 {
  position: relative;
  flex-direction: column;
  justify-content: center;
  height: 100%;
  width: auto;
  overflow: hidden;
  background-color: #0d0f27;
  min-width: 300px;
  justify-content: center;
}

/*  MODO -  2 */
.modo-2 {
  position: relative;
  flex-direction: column;
  justify-content: center;
  height: 100%;
  width: auto;
  overflow: hidden;
  background-color: #0d0f27;
  min-width: 300px;

}

/* CARDS- DADOS  */
.cards {
  display: grid;
  grid-auto-rows: max-content max-content;
  position: relative;
  justify-content: center;
  padding-top: 2%;
  overflow: auto;

}

/*  CARDS UNIT */
.card {
  background: #262525;
  color: #fff;
  padding: 1.5rem;
  overflow: hidden;
  border-radius: 0.8rem;
  width: 50vw;
  height: 50vw;
  min-height: 20px;
  max-height: 250px;
  min-width: 20px;
  max-width: 250px;
}

.card-title-large {
  font-family: fantasy, sans-serif;
  font-size: 3.5rem;
  text-align: right;
  letter-spacing: 0.5rem;
  color: #1d1d1d;
  user-select: none;
  transform: translateX(4rem);
  transition: transform 2.5s;
}

.card-title-small {
  margin-bottom: 0.25rem;
}


.card-description {
  font-size: 1rem;
  line-height: 1.5;
}

.card-cta {
  display: inline-block;
  width: 3.5rem;
  height: 3.5rem;
  border-radius: 50%;
  display: grid;
  place-items: center;
  margin-left: auto;
}

.card:hover .card-cta {
  background-color: #262525;
}

.card:hover .card-title-large {
  transform: translateX(-115%);
  color: #fff;
}

.card.humidity:hover {
  background-color: #1da1f2;
}

.card.servo:hover {
  background: radial-gradient(at 20% 128%,
      #a826b4 20%,
      #942a4a 60%,
      #971133 90%);
}

.card.solo:hover {
  background-color: #26790d;
}

.card.temperaturec:hover {
  background-color: #ff6207;
}

.card.temperaturef:hover {
  background-color: #5a025a;
}

.card.bomba:hover {
  background: radial-gradient(at 20% 128%,
      #250570 20%,
      #037488 60%,
      #8eb1c9 90%);
}

i {
  font-size: 2.4rem;
}

/*  MODO - MANUAL */

.eyes-off {
  display: grid;
  grid-auto-rows: max-content;
  position: relative;
  justify-content: center;
  padding-top: 2%;
  overflow: auto;
}

.eyes-off p {
  font-family: serif;
  padding: 0.4rem;
  font-size: 1.26rem;
  color: #ffffff;

}

.eyes-off #normal {
  color: #ffffff;
}

.eyes-off .slider {
  background-color: #1fb91f;
}



/*  MODO - AUTOMATICO */


.eyes-on #normal {
  color: #ebe9e9;
}

.eyes-on .slider {
  background-color: #ebe9e9;
}

.eyes-on {
  display: grid;
  grid-auto-rows: max-content;
  position: relative;
  justify-items: center;
  padding-top: 2%;
  overflow: auto;
}

.eyes-on p {
  font-family: serif;
  padding: 0.4rem;
  font-size: 1.26rem;
  color: #ffffff;

}

/*  BOTÃO - MODO  */

li {
  list-style: none;
  margin: 0;
  padding: 0;
}

.tg-list {
  text-align: center;
  display: flex;
  align-items: center;
}

.tg-list-item {
  display: grid;
  grid-template-columns: max-content max-content;
  align-items: center;
}

#list_b1 {
  column-gap: 1.4em;
}

.modo {
  color: #999;
}

.tgl {
  display: none;
}

.tgl,
.tgl:after,
.tgl:before,
.tgl *,
.tgl *:after,
.tgl *:before,
.tgl+.tgl-btn {
  box-sizing: border-box;
}

.tgl::selection,
.tgl:after::selection,
.tgl:before::selection,
.tgl *::selection,
.tgl *:after::selection,
.tgl *:before::selection,
.tgl+.tgl-btn::selection {
  background: none;
}

.tgl+.tgl-btn {
  outline: 0;
  display: block;
  width: 8em;
  height: 2em;
  position: relative;
  cursor: pointer;
  user-select: none;
}

.tgl+.tgl-btn:after,
.tgl+.tgl-btn:before {
  position: relative;
  display: block;
  content: "";
  width: 50%;
  height: 100%;
}

.tgl+.tgl-btn:after {
  left: 0;
}

.tgl+.tgl-btn:before {
  display: none;
}

.tgl:checked+.tgl-btn:after {
  left: 50%;
}

.tgl-flip+.tgl-btn {
  padding: 2px;
  transition: all 0.2s ease;
  font-family: sans-serif;
  perspective: 100px;
}

.tgl-flip+.tgl-btn:after,
.tgl-flip+.tgl-btn:before {
  display: inline-block;
  transition: all 0.6s ease;
  width: 100%;
  text-align: center;
  position: absolute;
  line-height: 2em;
  font-weight: bold;
  color: #fff;
  position: absolute;
  top: 0;
  left: 0;
  backface-visibility: hidden;
  border-radius: 4px;
}

.tgl-flip+.tgl-btn:after {
  content: attr(data-tg-on);
  background: #02c66f;
  transform: rotateY(-180deg);
}

.tgl-flip+.tgl-btn:before {
  background: #ff3a19;
  content: attr(data-tg-off);
}

.tgl-flip+.tgl-btn:active:before {
  transform: rotateY(-20deg);
}

.tgl-flip:checked+.tgl-btn:before {
  transform: rotateY(180deg);
}

.tgl-flip:checked+.tgl-btn:after {
  transform: rotateY(0);
  left: 0;
  background: #7fc6a6;
}

.tgl-flip:checked+.tgl-btn:active:after {
  transform: rotateY(20deg);
}




/* RANGE SLIDER DO SERVO  */
/* Config das bordas*/
.p-control {
  display: grid;
  grid-template-rows: max-content;
  row-gap: 1.5em;
  border: 0.1rem solid red;
  border-radius: 4em;
  justify-items: center;
  padding: 6%;
}


.container_data {
  grid-template-rows: max-content;
  row-gap: 6%;
  justify-items: center;

}


#rangeValue {
  display: flex;
  justify-content: center;
  text-align: center;
  font-size: 6em;
  color: #999;
  font-weight: 400;
}

.range {
  width: 400px;
  height: 15px;
  appearance: none;
  -webkit-appearance: none;
  background: #111;
  outline: none;
  border-radius: 15px;
  overflow: hidden;
  box-shadow: inset 0 0 5px rgba(0, 0, 0, 1);
}

.range::-webkit-slider-thumb {
  -webkit-appearance: none;
  width: 15px;
  height: 15px;
  border-radius: 50%;
  background: #00fd0a;
  cursor: pointer;
  border: 4px solid #333;
  box-shadow: -407px 0 0 400px #00fd0a;
}


/*  BOTÃO SUBMIT - SERVO */

@import url(https://fonts.googleapis.com/css?family=Space+Mono);

* {
  box-sizing: border-box;
}

small {
  color: #888;
}

@media (min-width: 780px) {
  .wrapper {
    width: 600px;
    display: grid;
    grid-template-columns: repeat(2, [col] calc(100%/2));
    grid-auto-rows: 120px;
    margin: 30px auto 40px;
  }
}

.button {
  display: inline-block;
  min-width: 150px;
  margin: 20px auto;
  background: #8bc34a;
  color: #fefefe;
  font-size: 1.2em;
  padding: 1em;
  border-radius: 4px;
  text-align: center;
  position: relative;
  cursor: pointer;
  appearance: none;
  -webkit-appearance: none;
  border: 0;
  transition: border-radius linear 0.05s, width linear 0.05s;
}

.button:focus {
  outline: 0;
}

.button.animate {
  width: 68.1818181818px;
  height: 68.1818181818px;
  min-width: 0;
  border-radius: 50%;
  color: transparent;
}

.button.animate:after {
  position: absolute;
  content: '';
  width: 25px;
  height: 25px;
  border: 4px solid #fefefe;
  border-radius: 50%;
  border-left-color: transparent;
  left: 50%;
  top: 50%;
  -webkit-transform: translate(-50%, -50%);
  transform: translate(-50%, -50%);
  animation: spin ease-in 2.5s forwards;
  animation-name: spin;
  -webkit-animation-name: spin;
  transition-timing-function: ease-in-out;
  -webkit-transition-timing-function: ease-in-out;
  animation-duration: 2.5s;
  -webkit-animation-duration: 2.5s;
  animation-fill-mode: forwards;
  -webkit-animation-fill-mode: forwards;
}

.button.animate.success:before {
  position: absolute;
  content: '';
  width: 25px;
  height: 12.5px;
  border: 4px solid #fefefe;
  border-right: 0;
  border-top: 0;
  left: 50%;
  top: 50%;
  -webkit-transform: translate(-50%, -50%) rotate(0deg) scale(0);
  transform: translate(-50%, -50%) rotate(0deg) scale(0);
  -webkit-animation: success ease-in 0.15s forwards;
  animation: success ease-in 0.15s forwards;
  animation-delay: 2.5s;
}

.button.animate.error {
  position: relative;
  -webkit-animation: vibrate ease-in 0.5s forwards;
  animation: vibrate ease-in 0.5s forwards;
  -webkit-animation-delay: 2.5s;
  animation-delay: 2.5s;
}

.button.animate.error:before {
  color: #fff;
  position: absolute;
  content: '!';
  font-size: 1.8rem;
  font-weight: bold;
  text-align: center;
  left: 50%;
  top: 50%;
  -webkit-transform: translate(-50%, -50%) scale(0);
  transform: translate(-50%, -50%) scale(0);
  -webkit-animation: error ease-in 0.5s forwards;
  animation: error ease-in 0.5s forwards;
  animation-delay: 2.5s;
}

@keyframes spin {
  0% {
    transform: translate(-50%, -50%) rotate(0deg) scale(1);
  }

  90% {
    transform: translate(-50%, -50%) rotate(1080deg) scale(1);
  }

  100% {
    transform: scale(0);
  }
}

@-webkit-keyframes spin {
  0% {
    -webkit-transform: translate(-50%, -50%) rotate(0deg) scale(1);
  }

  98% {
    -webkit-transform: translate(-50%, -50%) rotate(1080deg) scale(1);
  }

  100% {
    -webkit-transform: translate(-50%, -50%) rotate(1080deg) scale(0);
  }
}

@keyframes success {
  from {
    transform: translate(-50%, -50%) rotate(0) scale(0);
  }

  to {
    transform: translate(-50%, -50%) rotate(-45deg) scale(1);
  }
}

@-webkit-keyframes success {
  from {
    -webkit-transform: translate(-50%, -50%) rotate(0) scale(0);
  }

  to {
    -webkit-transform: translate(-50%, -50%) rotate(-45deg) scale(1);
  }
}

@keyframes error {
  from {
    transform: translate(-50%, -50%) scale(0);
  }

  to {
    transform: translate(-50%, -50%) scale(1);
    background-color: #f44336;
  }
}

@-webkit-keyframes error {
  from {
    -webkit-transform: translate(-50%, -50%) scale(0);
  }

  to {
    -webkit-transform: translate(-50%, -50%) scale(1);
    background-color: #f44336;
  }
}

@keyframes vibrate {

  0%,
  30%,
  60%,
  85%,
  100% {
    left: 0;
    background-color: #f44336;
  }

  10%,
  40%,
  90%,
  70% {
    left: -2px;
    background-color: #f44336;
  }

  20%,
  50%,
  80%,
  95% {
    left: 2px;
    background-color: #f44336;
  }
}




/* BOTÃO A BOMBA D'AGUA  */




.tgl-skewed+.tgl-btn {
  overflow: hidden;
  transform: skew(-10deg);
  backface-visibility: hidden;
  transition: all 0.2s ease;
  font-family: sans-serif;
  background: #888;
}

.tgl-skewed+.tgl-btn:after,
.tgl-skewed+.tgl-btn:before {
  transform: skew(10deg);
  display: inline-block;
  transition: all 0.2s ease;
  width: 100%;
  text-align: center;
  position: absolute;
  line-height: 2em;
  font-weight: bold;
  color: #fff;
  text-shadow: 0 1px 0 rgba(0, 0, 0, .4);
}

.tgl-skewed+.tgl-btn:after {
  left: 100%;
  content: attr(data-tg-on);
}

.tgl-skewed+.tgl-btn:before {
  left: 0;
  content: attr(data-tg-off);
}

.tgl-skewed+.tgl-btn:active {
  background: #888;
}

.tgl-skewed+.tgl-btn:active:before {
  left: -10%;
}

.tgl-skewed:checked+.tgl-btn {
  background: #86d993;
}

.tgl-skewed:checked+.tgl-btn:before {
  left: -100%;
}

.tgl-skewed:checked+.tgl-btn:after {
  left: 0;
}

.tgl-skewed:checked+.tgl-btn:active:after {
  left: 10%;
}




/*  CARDS ATUADORES */

.dados_extras {

  display: grid;
  grid-auto-rows: max-content;
  position: relative;
  justify-content: center;
  overflow: auto;


}

/*  MENU DOWNLOADS */

.baixar-dados {
  
  display: grid;
  grid-auto-rows: max-content;
  position: relative;
  justify-content: center;
  gap: 7%;
  padding-top: 2%;
  overflow: auto;
  height: 90%;
}


.titulo-medio {
  color: #00fd0a;

}

.exportar {
  position: relative;
  background: #444;
  color: #fff;
  text-decoration: none;
  text-transform: uppercase;
  border: none;
  letter-spacing: 0.1rem;
  font-size: 1rem;
  padding: 1rem 3rem;
  transition: 0.2s;
}

.exportar:hover {
  letter-spacing: 0.2rem;
  padding: 1.1rem 3.1rem;
  background: var(--clr);
  color: var(--clr);
  /* box-shadow: 0 0 35px var(--clr); */
  animation: box 3s infinite;
}

.exportar::before {
  content: "";
  position: absolute;
  inset: 2px;
  background: #272822;
}

.exportar span {
  position: relative;
  z-index: 1;
}

.exportari {
  position: absolute;
  inset: 0;
  display: block;
}

.exportari::before {
  content: "";
  position: absolute;
  width: 10px;
  height: 2px;
  left: 80%;
  top: -2px;
  border: 2px solid var(--clr);
  background: #272822;
  transition: 0.2s;
}

.exportar:hover i::before {
  width: 15px;
  left: 20%;
  animation: move 3s infinite;
}

.exportar i::after {
  content: "";
  position: absolute;
  width: 10px;
  height: 2px;
  left: 20%;
  bottom: -2px;
  border: 2px solid var(--clr);
  background: #272822;
  transition: 0.2s;
}

.exportar:hover i::after {
  width: 15px;
  left: 80%;
  animation: move 3s infinite;
}

@keyframes move {
  0% {
    transform: translateX(0);
  }

  50% {
    transform: translateX(5px);
  }

  100% {
    transform: translateX(0);
  }
}

@keyframes box {
  0% {
    box-shadow: #27272c;
  }

  50% {
    box-shadow: 0 0 25px var(--clr);
  }

  100% {
    box-shadow: #27272c;
  }
}



/*  FOOTER */

.footer {

  height: 85vw;
  min-height: 100px;
  background-color: #151823;
  min-width: 300px;

}


.footer__title {
  font-size: 1.8rem;
  color: #fff;
  font-weight: 500;
  margin-bottom: 2rem;
}

.footer__list {
  display: flex;
  flex-direction: column;
  gap: 2rem;

}

.footer__list-item {
  list-style: none;
}

.footer__list-link {
  text-decoration: none;
  color: inherit;
  transition: color .25s;
}

.footer__list-link:hover {
  color: #fff;
}

.project__info {
  color: #666873;
  position: relative;
  overflow: hidden;
  display: flex;
  flex-wrap: wrap;
  justify-content: center;
  align-items: center;
  align-content: flex-start;

}

.company__logo {
  color: #fff;
}

.project__info ul {
  flex-direction: row;
}

.dados_project {
  display: grid;
  grid-template-columns: max-content max-content;

}

.project__description {
  font-size: 0.9rem;
  text-align: center;
}

.copyright {
  font-size: 0.9rem;

}


/*  RESPOSIVIDADE   */

@media (min-width:350px) and (max-width:400px) {
  /*HEADER*/

  
  .header:before {
    background-size: 200%;
  }

  .info {
    display: flex;
    flex-direction: column;
    align-items: center;
    padding: 3%;

  }

  .info h4 {
    font-size: 0.9em
  }

  .info h1 {
    font-size: 2em;
  }

  /*MENU*/


  .tablink {
    font-size: 0.8em;
  }

  /*CARDS*/

  .cards {
    padding: 6%;
    gap: 10%;
    justify-content: center;
  }

  .card {
    width: 9em;
    height: 9em;
  }

  .card-title-large {

    font-size: 2em;
  }

  .card-title-small {
    font-size: 0.8em;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .card-title-small i {
    font-size: 1.4em;
  }


  /*ATUADORES*/
  .dados_extras {


    gap: 6%;
    padding: 6%;


  }

  /*Config*/

  .titulos {

    font-size: 1.3em;
    text-align: center;
  }

  #rangeValue {
    font-size: 2.5em;
  }

  .range {
    width: 70vw;
  }

  .p-control {
    border-radius: 1em;
  }

  #body {

    padding: 6%;
    gap: 2%;
  }

  .container_data {
    row-gap: 3%;

  }

  /* Dowloads*/

  .grid-btn{
    display: grid;
    grid-template-columns: max-content ;
    gap:6%;

  }


  .baixar-dados {
    padding: 6%;
    justify-items: center;
  }

  /*FOOTER*/
  .footer{

    height: min-content;
    overflow: auto;
  }
  .company__logo {

    font-size: 1.2em;
  }

  .dados_project i {
    font-size: 1.5em;
  }

  .project__description {
    font-size: 0.9em;
  }

  .copyright {
    font-size: 0.9em;
  }

  .dados_project {
    align-items: center;
  }

  .project__info {
    display: grid;
    grid-template-rows: max-content;
    justify-items: center;
    overflow: auto;
    padding: 6%;
  }

  /*FINALIZAÇÃOS*/

}

@media (min-width:401px) and (max-width:500px) {

  .header:before {
    background-size: 200%;
  }

  .info {
    padding: 6%;
    width: 100%;

  }

  .info h4 {

    font-size: 1em
  }

  .info h1 {
    font-size: 1.9em;
  }

  /*MENU*/


  .tablink {
    font-size: 0.9em;
  }

  /*CARDS*/

  .cards {
    padding: 6%;
    gap: 10%;
    justify-content: center;
  }

  .card {
    width: 12em;
    height: 12em;
  }

  .card-title-large {

    font-size: 2.3em;
  }

  .card-title-small {
    font-size: 1em;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .card-title-small i {
    font-size: 2.1em;
  }


  /*ATUADORES*/
  .dados_extras {


    gap: 6%;
    padding: 6%;


  }

  /*Config*/

  .titulos {

    font-size: 1.8em;
    text-align: center;
  }

  .modo {
    font-size: 1.8em;
  }

  #rangeValue {
    font-size: 2.8em;
  }

  .range {
    width: 70vw;
  }

  .p-control {
    border-radius: 1em;
  }

  #body {

    padding: 6%;
    gap: 2%;
  }

  .tgl+.tgl-btn {
    height: 2.1em;
    width: 8em;
    font-size: 1.8em;
  }



  .container_data {
    row-gap: 3%;

  }

  /* Dowloads*/
  .grid-btn{
    display: grid;
    grid-template-columns: max-content ;
    gap:6%;

  }


  .baixar-dados {
    padding: 6%;
    justify-items: center;
  }


  .titulo-medio {
    font-size: 1.4em;
  }

  .exportar span {
    font-size: 1.3em;
  }

  /*FOOTER*/
  .footer{
    height: min-content;
    overflow: auto;
  }
  .company__logo {

    font-size: 1.6em;
  }

  .dados_project i {
    font-size: 1.8em;
  }

  .project__description {
    font-size: 1em;
  }

  .copyright {
    font-size: 0.9em;
  }

  .dados_project {
    align-items: center;
  }

  .project__info {
    display: grid;
    grid-template-rows: max-content;
    justify-items: center;
    overflow: auto;
    padding: 6%;
  }



  /*FINALIZAÇÃOS*/

}

@media (min-width:501px) and (max-width:600px) {
  header:before {
    background-size: 200%;
  }

  .info {
    padding: 6%;
    width: 100%;

  }

  .info h4 {

    font-size: 1.3em
  }

  .info h1 {
    font-size: 1.9em;
  }

  /*MENU*/


  .tablink {
    font-size: 1.1em;
  }

  /*CARDS*/

  .cards {
    padding: 6%;
    gap: 10%;
    justify-content: center;
  }

  .card {
    width: 14em;
    height: 14em;
  }

  .card-title-large {

    font-size: 2.6em;
  }

  .card-title-small {
    font-size: 1.3em;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .card-title-small i {
    font-size: 2.2em;
  }


  /*ATUADORES*/
  .dados_extras {


    gap: 6%;
    padding: 6%;


  }

  /*Config*/

  .titulos {

    font-size: 1.8em;
    text-align: center;
  }

  .modo {
    font-size: 2em;
  }

  #rangeValue {
    font-size: 3em;
  }

  .range {
    width: 70vw;
  }

  .p-control {
    border-radius: 1em;
  }

  #body {

    padding: 6%;
    gap: 2%;
  }

  .tgl+.tgl-btn {
    height: 2.1em;
    width: 8em;
    font-size: 1.8em;
  }



  .container_data {
    row-gap: 3%;

  }

  /* Dowloads*/
  .grid-btn{
    display: grid;
    grid-template-columns: max-content ;
    gap:6%;

  }

  .baixar-dados {
    padding: 6%;
    justify-items: center;
  }


  .titulo-medio {
    font-size: 1.5em;
  }

  .exportar span {
    font-size: 1.4em;
  }

  /*FOOTER*/
  .footer{
   height: min-content;
   overflow: auto;

  }
  .company__logo {

    font-size: 1.7em;
  }

  .dados_project i {
    font-size: 1.9em;
  }

  .project__description {
    font-size: 1.2em;
  }

  .copyright {
    font-size: 1em;
  }

  .dados_project {
    align-items: center;
  }

  .project__info {
    display: grid;
    grid-template-rows: max-content;
    justify-items: center;
    overflow: auto;
    padding: 6%;
  }


  /*FINALIZAÇÃOS*/
}

@media (min-width:601px) and (max-width:700px) {
  header:before {
    background-size: 200%;
  }

  .info {
    padding: 6%;
    width: 100%;

  }

  .info h4 {

    font-size: 1.3em
  }

  .info h1 {
    font-size: 1.9em;
  }

  /*MENU*/


  .tablink {
    font-size: 1.1em;
  }

  /*CARDS*/

  .cards {
    padding: 6%;
    gap: 10%;
    justify-content: center;
  }

  .card {
    width: 14em;
    height: 14em;
  }

  .card-title-large {

    font-size: 2.6em;
  }

  .card-title-small {
    font-size: 1.3em;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .card-title-small i {
    font-size: 1.7em;
  }


  /*ATUADORES*/
  .dados_extras {


    gap: 6%;
    padding: 6%;


  }

  /*Config*/

  .titulos {

    font-size: 1.8em;
    text-align: center;
  }

  .modo {
    font-size: 2em;
  }

  #rangeValue {
    font-size: 3em;
  }

  .range {
    width: 70vw;
  }

  .p-control {
    border-radius: 1em;
  }

  #body {

    padding: 6%;
    gap: 2%;
  }

  .tgl+.tgl-btn {
    height: 2.1em;
    width: 8em;
    font-size: 1.8em;
  }



  .container_data {
    row-gap: 3%;

  }

  /* Dowloads*/
  .grid-btn{
    display: grid;
    grid-template-columns: max-content ;
    gap:6%;

  }

  .baixar-dados {
    padding: 6%;
    justify-items: center;
  }

  
  .titulo-medio {
    font-size: 1.5em;
  }

  .exportar span {
    font-size: 1.4em;
  }

  /*FOOTER*/
  .footer{
    height: min-content;
    overflow: auto;
  }
  .company__logo {

    font-size: 1.7em;
  }

  .dados_project i {
    font-size: 1.9em;
  }

  .project__description {
    font-size: 1.2em;
  }

  .copyright {
    font-size: 1em;
  }

  .dados_project {
    align-items: center;
  }

  .project__info {
    display: grid;
    grid-template-rows: max-content;
    justify-items: center;
    overflow: auto;
    padding: 6%;
  }

  /*FINALIZAÇÃOS*/
}

@media (min-width:701px) and (max-width:800px) {
  header:before {
    background-size: 200%;
  }

  .info {
    padding: 6%;
    width: 100%;

  }

  .info h4 {

    font-size: 1.5em
  }

  .info h1 {
    font-size: 2em;
  }

  /*MENU*/


  .tablink {
    font-size: 1.5em;
  }

  /*CARDS*/

  .cards {
    padding: 6%;
    gap: 10%;
    justify-content: center;
    grid-template-columns: max-content max-content;
  }

  .card {
    width: 16em;
    height: 16em;
  }

  .card-title-large {

    font-size: 2.8m;
  }

  .card-title-small {
    font-size: 1.5em;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .card-title-small i {
    font-size: 1.6em;
  }


  /*ATUADORES*/
  .dados_extras {


    gap: 6%;
    padding: 6%;
    grid-template-columns: max-content max-content;

  }

  /*Config*/

  .titulos {

    font-size: 3em;
    text-align: center;
  }

  .modo {
    font-size: 2.4em;
  }

  #rangeValue {
    font-size: 34m;
  }

  .range {
    width: 70vw;
  }

  .p-control {
    border-radius: 1em;
  }

  #body {

    padding: 6%;
    gap: 2%;
  }

  .tgl+.tgl-btn {
    height: 2em;
    width: 6em;
    font-size: 2.3em;
  }

  #btn1 {
    font-size: 2em;
  }


  .container_data {
    row-gap: 3%;

  }

  /* Dowloads*/
  .grid-btn{
    display: grid;
    grid-template-columns: max-content ;
    gap:6%;

  }

  .baixar-dados {
    padding: 6%;
    justify-items: center;
  }

  

  .titulo-medio {
    font-size: 1.9em;
  }

  .exportar span {
    font-size: 1.6em;
  }

  /*FOOTER*/

  .footer{
    height: min-content;
    overflow: auto;
  }
  .company__logo {

    font-size: 2em;
  }

  .dados_project i {
    font-size: 2.4em;
  }

  .dados_project {
    align-items: center;
  }

  .project__description {
    font-size: 1.2em;
  }

  .copyright {
    font-size: 1em;
  }

  .project__info {
    display: grid;
    grid-template-rows: max-content;
    justify-items: center;
    overflow: auto;
    padding: 5%;
  }

  /*FINALIZAÇÃOS*/

}

@media (min-width:801px) and (max-width:900px) {
  header:before {
    background-size: 200%;
  }

  .info {
    padding: 6%;
    width: 100%;

  }

  .info h4 {

    font-size: 1.6em
  }

  .info h1 {
    font-size: 2.5em;
  }

  /*MENU*/


  .tablink {
    font-size: 1.5em;
  }

  /*CARDS*/

  .cards {
    padding: 6%;
    gap: 10%;
    justify-content: center;
    grid-template-columns: max-content max-content;
  }

  .card {
    width: 17em;
    height: 17em;
  }

  .card-title-large {

    font-size: 2.8m;
  }

  .card-title-small {
    font-size: 1.7em;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .card-title-small i {
    font-size: 1.4em;
  }


  /*ATUADORES*/
  .dados_extras {


    gap: 6%;
    padding: 6%;
    grid-template-columns: max-content max-content;

  }

  /*Config*/

  .titulos {

    font-size: 3em;
    text-align: center;
  }

  .modo {
    font-size: 2.4em;
  }

  #rangeValue {
    font-size: 34m;
  }

  .range {
    width: 70vw;
  }

  .p-control {
    border-radius: 1em;
  }

  #body {

    padding: 6%;
    gap: 2%;
  }

  .tgl+.tgl-btn {
    height: 2em;
    width: 6em;
    font-size: 2.3em;
  }

  #btn1 {
    font-size: 2em;
  }


  .container_data {
    row-gap: 3%;

  }

  /* Dowloads*/
  .grid-btn{
    display: grid;
    grid-template-columns: max-content ;
    gap:6%;

  }

  .baixar-dados {
    padding: 6%;
    justify-items: center;
  }


  .titulo-medio {
    font-size: 2em;
  }

  .exportar span {
    font-size: 1.8em;
  }


  /*FOOTER*/

  .footer{
    height: min-content;
    overflow: auto;
  }
  .company__logo {

    font-size: 2.3em;
  }

  .dados_project i {
    font-size: 2.5em;
  }

  .dados_project {
    align-items: center;
  }

  .project__description {
    font-size: 1.5em;
  }

  .copyright {
    font-size: 1.3em;
  }

  .project__info {
    display: grid;
    grid-template-rows: max-content;
    justify-items: center;
    overflow: auto;
    padding: 5%;
  }

  /*FINALIZAÇÃOS*/


}

@media (min-width:901px) and (max-width:1000px) {
  header:before {
    background-size: 200%;
  }

  .info {
    padding: 4%;
    width: 100%;

  }

  .info h4 {

    font-size: 1.6em
  }

  .info h1 {
    font-size: 2.5em;
  }

  /*MENU*/


  .tablink {
    font-size: 1.6em;
  }

  /*CARDS*/

  .cards {
    padding: 6%;
    gap: 10%;
    justify-content: center;
    grid-template-columns: max-content max-content;
  }

  .card {
    width: 19em;
    height: 19em;
  }

  .card-title-large {

    font-size: 2.8m;
  }

  .card-title-small {
    font-size: 1.7em;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .card-title-small i {
    font-size: 1.4em;
  }


  /*ATUADORES*/
  .dados_extras {


    gap: 6%;
    padding: 6%;
    grid-template-columns: max-content max-content;

  }

  /*Config*/

  .titulos {

    font-size: 3em;
    text-align: center;
  }

  .modo {
    font-size: 2.4em;
  }

  #rangeValue {
    font-size: 34m;
  }

  .range {
    width: 70vw;
  }

  .p-control {
    border-radius: 1em;
  }

  #body {

    padding: 6%;
    gap: 2%;
  }

  .tgl+.tgl-btn {
    height: 2em;
    width: 6em;
    font-size: 2.3em;
  }

  #btn1 {
    font-size: 2em;
  }


  .container_data {
    row-gap: 3%;

  }

  /* Dowloads*/
  .grid-btn{
    display: grid;
    grid-template-columns: max-content ;
    gap:6%;

  }

  .baixar-dados {
    padding: 6%;
    justify-items: center;
  }

 

  .titulo-medio {
    font-size: 2em;
  }

  .exportar span {
    font-size: 1.8em;
  }

  /*FOOTER*/
  .footer{

    height:min-content;
    overflow:auto;
  }
  .company__logo {

    font-size: 2.3em;
  }

  .dados_project i {
    font-size: 2.5em;
  }

  .dados_project {
    align-items: center;
  }

  .project__description {
    font-size: 1.5em;
  }

  .copyright {
    font-size: 1.3em;
  }

  .project__info {
    display: grid;
    grid-template-rows: max-content;
    justify-items: center;
    overflow: auto;
    padding: 5%;
  }

  /*FINALIZAÇÃOS*/
}

@media (min-width:1001px) and (max-width:1100px) {

 .header {
    height: min-content;
  }

  header:before {
    background-size: 100%;
  }

  .info {
    padding: 3%;
    width: 100%;

  }

  .info h4 {

    font-size: 1.6em
  }

  .info h1 {
    font-size: 2.5em;
  }

  /*MENU*/


  .tablink {
    font-size: 1.6em;
  }

  /*CARDS*/

  .cards {
    padding: 3%;
    gap: 5%;
    justify-content: center;
    grid-template-columns: max-content max-content max-content max-content;
    overflow: hidden;
  }

  .card {
    width: 13em;
    height: 13em;
  }

  .card-title-large {

    font-size: 2.2m;
  }

  .card-title-small {
    font-size: 1.2em;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .card-title-small i {
    font-size: 1em;
  }


  /*ATUADORES*/
  .dados_extras {


    gap: 6%;
    padding: 4%;
    grid-template-columns: max-content max-content;
    overflow: hidden;

  }

  /*Config*/

  .titulos {

    font-size: 1.8em;
    text-align: center;
  }

  .modo {
    font-size: 1.8em;
  }

  #rangeValue {
    font-size: 2em;
  }

  .range {
    width: 40vw;
  }

  .p-control {
    border-radius: 1em;
    row-gap:0.1em;
  }

  #body {

    padding: 3%;
    gap: 1%;
  }

  .tgl+.tgl-btn {
    height: 2.1em;
    width: 6em;
    font-size: 2em;
  }

  #btn1 {
    font-size: 1em;
  }


  .container_data {
    grid-template-columns: max-content max-content ;
    column-gap: 8%;
    padding: 0.1%;

  }

  /* Dowloads*/
  .baixar-dados {
   
    padding: 6%;
    gap: 8%;
    justify-items: center;
    overflow: hidden;
  }

  .titulo-medio {
    font-size: 2em;
  }
   
  .exportar span {
    font-size: 1.8em;
  }
  .grid-btn{
    display: grid;
    grid-template-columns: max-content max-content max-content max-content;
    gap:6%;

  }

  /*FOOTER*/
  .footer {
    height: max-content;

  }

  .company__logo {

    font-size: 1.2em;
  }

  .dados_project i {
    font-size: 1.4em;
  }

  .dados_project {
    align-items: center;
  }

  .project__description {
    font-size: 1em;
  }

  .copyright {
    font-size: 1em;
  }

  .project__info {
    display: grid;
    grid-template-columns: max-content max-content max-content;
    gap: 2%;
    justify-items: center;
    overflow: hidden;
    padding: 1%;
    height: 100%;
  }
  /*FINALIZAÇÃOS*/
}
@media (min-width:1101px) and (max-width:1200px) {

  .header {
     height: min-content;
   }
 
   header:before {
     background-size: 100%;
   }
 
   .info {
     padding: 3%;
     width: 100%;
 
   }
 
   .info h4 {
 
     font-size: 1.6em
   }
 
   .info h1 {
     font-size: 2.5em;
   }
 
   /*MENU*/
 
 
   .tablink {
     font-size: 1.6em;
   }
 
   /*CARDS*/
 
   .cards {
     padding: 3%;
     gap: 5%;
     justify-content: center;
     grid-template-columns: max-content max-content max-content max-content;
     overflow: hidden;
   }
 
   .card {
     width: 13em;
     height: 13em;
   }
 
   .card-title-large {
 
     font-size: 2.2m;
   }
 
   .card-title-small {
     font-size: 1.2em;
     display: flex;
     justify-content: center;
     align-items: center;
   }
 
   .card-title-small i {
     font-size: 1em;
   }
 
 
   /*ATUADORES*/
   .dados_extras {
 
 
     gap: 6%;
     padding: 4%;
     grid-template-columns: max-content max-content;
     overflow: hidden;
 
   }
 
   /*Config*/
 

  .titulos {

    font-size: 1.8em;
    text-align: center;
  }

  .modo {
    font-size: 1.8em;
  }

  #rangeValue {
    font-size: 2em;
  }

  .range {
    width: 40vw;
  }

  .p-control {
    border-radius: 1em;
    row-gap:0.1em;
  }

  #body {

    padding: 3%;
    gap: 1%;
  }

  .tgl+.tgl-btn {
    height: 2.1em;
    width: 6em;
    font-size: 2em;
  }

  #btn1 {
    font-size: 1em;
  }


  .container_data {
    grid-template-columns: max-content max-content ;
    column-gap: 8%;
    padding: 0.1%;

  }
   
 
   /* Dowloads*/
   .baixar-dados {
    
     padding: 6%;
     gap: 8%;
     justify-items: center;
     overflow: hidden;
   }
 
   .titulo-medio {
     font-size: 2em;
   }
    
   .exportar span {
     font-size: 1.8em;
   }
   .grid-btn{
     display: grid;
     grid-template-columns: max-content max-content max-content max-content;
     gap:6%;
 
   }
 
   /*FOOTER*/
   .footer {
     height: max-content;
 
   }
 
   .company__logo {
 
     font-size: 1.2em;
   }
 
   .dados_project i {
     font-size: 1.4em;
   }
 
   .dados_project {
     align-items: center;
   }
 
   .project__description {
     font-size: 1em;
   }
 
   .copyright {
     font-size: 1em;
   }
 
   .project__info {
     display: grid;
     grid-template-columns: max-content max-content max-content;
     gap: 2%;
     justify-items: center;
     overflow: hidden;
     padding: 1%;
     height: 100%;
   }
   /*FINALIZAÇÃOS*/
 }
 
@media (min-width:1200px) and (max-width:1300px) {
  .header {
    height: min-content;
  }

  header:before {
    background-size: 100%;
  }

  .info {
    padding: 3%;
    width: 100%;

  }

  .info h4 {

    font-size: 1.6em
  }

  .info h1 {
    font-size: 2.5em;
  }

  /*MENU*/


  .tablink {
    font-size: 1.6em;
  }

  /*CARDS*/

  .cards {
    padding: 4%;
    gap: 7%;
    justify-content: center;
    grid-template-columns: max-content max-content max-content max-content;
    overflow: hidden;
  }

  .card {
    width: 14em;
    height: 14em;
  }

  .card-title-large {

    font-size: 2.4m;
  }

  .card-title-small {
    font-size: 1.4em;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .card-title-small i {
    font-size: 1.3em;
  }


  /*ATUADORES*/
  .dados_extras {


    gap: 6%;
    padding: 4%;
    grid-template-columns: max-content max-content;
    overflow: hidden;

  }

  /*Config*/
  .titulos {

    font-size: 1.5em;
    text-align: center;
  }

  .modo {
    font-size: 1.5em;
  }

  #rangeValue {
    font-size: 1.8em;
  }

  .range {
    width: 40vw;
  }

  .p-control {
    border-radius: 1em;
    row-gap:0.1em;
  }

  #body {

    padding: 3%;
    gap: 1%;
  }

  .tgl+.tgl-btn {
    height: 2.1em;
    width: 6em;
    font-size: 1,7em;
  }

  #btn1 {
    font-size: 1em;
  }


  .container_data {
    grid-template-columns: max-content max-content ;
    column-gap: 8%;
   padding: 0.1%;

  }
 

  /* Dowloads*/
  .baixar-dados {
   
    padding: 6%;
    gap: 8%;
    justify-items: center;
    overflow: hidden;
  }

  .titulo-medio {
    font-size: 2em;
  }
   
  .exportar span {
    font-size: 1.8em;
  }
  .grid-btn{
    display: grid;
    grid-template-columns: max-content max-content max-content max-content;
    gap:6%;

  }

  /*FOOTER*/
  .footer {
    height: max-content;

  }

  .company__logo {

    font-size: 1.9em;
  }

  .dados_project i {
    font-size: 2em;
  }

  .dados_project {
    align-items: center;
  }

  .project__description {
    font-size: 1em;
  }

  .copyright {
    font-size: 1em;
  }

  .project__info {
    display: grid;
    grid-template-columns: max-content max-content max-content;
    gap: 2%;
    justify-items: center;
    overflow: hidden;
    padding: 1%;
    height: 100%;
  }

  /*FINALIZAÇÃOS*/
}

@media (min-width:1301px) and (max-width:1400px) {
  .header {
    height: min-content;
  }

  header:before {
    background-size: 100%;
  }

  .info {
    padding: 3%;
    width: 100%;

  }

  .info h4 {

    font-size: 1.6em
  }

  .info h1 {
    font-size: 2.5em;
  }

  /*MENU*/


  .tablink {
    font-size: 1.6em;
  }

  /*CARDS*/

  .cards {
    padding: 6%;
    gap: 7%;
    justify-content: center;
    grid-template-columns: max-content max-content max-content max-content;
    overflow: hidden;
  }

  .card {
    width: 14em;
    height: 14em;
  }

  .card-title-large {

    font-size: 2.4m;
  }

  .card-title-small {
    font-size: 1.4em;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .card-title-small i {
    font-size: 1.3em;
  }


  /*ATUADORES*/
  .dados_extras {


    gap: 6%;
    padding: 6%;
    grid-template-columns: max-content max-content;
    overflow: hidden;

  }

  /*Config*/
  .titulos {

    font-size: 1.5em;
    text-align: center;
  }

  .modo {
    font-size: 1.5em;
  }

  #rangeValue {
    font-size: 1.8em;
  }

  .range {
    width: 40vw;
  }

  .p-control {
    border-radius: 1em;
    row-gap:0.1em;
  }

  #body {

    padding: 3%;
    gap: 1%;
  }

  .tgl+.tgl-btn {
    height: 2.1em;
    width: 6em;
    font-size: 1,7em;
  }

  #btn1 {
    font-size: 1em;
  }


  .container_data {
    grid-template-columns: max-content max-content ;
    column-gap: 8%;
   padding: 0.1%;

  }

  /* Dowloads*/
  .baixar-dados {
   
    padding: 6%;
    gap: 8%;
    justify-items: center;
    overflow: hidden;
  }

  .titulo-medio {
    font-size: 2em;
  }
   
  .exportar span {
    font-size: 1.8em;
  }
  .grid-btn{
    display: grid;
    grid-template-columns: max-content max-content max-content max-content;
    gap:6%;

  }

  /*FOOTER*/
  .footer {
    height: max-content;

  }

  .company__logo {

    font-size: 1.9em;
  }

  .dados_project i {
    font-size: 2em;
  }

  .dados_project {
    align-items: center;
  }

  .project__description {
    font-size: 1em;
  }

  .copyright {
    font-size: 1em;
  }

  .project__info {
    display: grid;
    grid-template-columns: max-content max-content max-content;
    gap: 2%;
    justify-items: center;
    overflow: hidden;
    padding: 1%;
    height: 100%;
  }

  /*FINALIZAÇÃOS*/
}

@media (min-width:1401px) {
  .header {
    height: min-content;
  }

  header:before {
    background-size: 100%;
  }

  .info {
    padding: 3%;
    width: 100%;

  }

  .info h4 {

    font-size: 1.6em
  }

  .info h1 {
    font-size: 2.5em;
  }

  /*MENU*/


  .tablink {
    font-size: 1.6em;
  }

  /*CARDS*/

  .cards {
    padding: 6%;
    gap: 7%;
    justify-content: center;
    grid-template-columns: max-content max-content max-content max-content;
    overflow: hidden;
  }

  .card {
    width: 14em;
    height: 14em;
  }

  .card-title-large {

    font-size: 2.4m;
  }

  .card-title-small {
    font-size: 1.4em;
    display: flex;
    justify-content: center;
    align-items: center;
  }

  .card-title-small i {
    font-size: 1.3em;
  }


  /*ATUADORES*/
  .dados_extras {


    gap: 6%;
    padding: 6%;
    grid-template-columns: max-content max-content;
    overflow: hidden;

  }

  /*Config*/
  .titulos {

    font-size: 1.5em;
    text-align: center;
  }

  .modo {
    font-size: 1.5em;
  }

  #rangeValue {
    font-size: 1.8em;
  }

  .range {
    width: 40vw;
  }

  .p-control {
    border-radius: 1em;
    row-gap:0.1em;
  }

  #body {

    padding: 3%;
    gap: 1%;
  }

  .tgl+.tgl-btn {
    height: 2.1em;
    width: 6em;
    font-size: 1,7em;
  }

  #btn1 {
    font-size: 1em;
  }


  .container_data {
    grid-template-columns: max-content max-content ;
    column-gap: 8%;
   padding: 0.1%;

  }
 
  /* Dowloads*/
  .baixar-dados {
   
    padding: 6%;
    gap: 8%;
    justify-items: center;
    overflow: hidden;
  }

  .titulo-medio {
    font-size: 2em;
  }
   
  .exportar span {
    font-size: 1.8em;
  }
  .grid-btn{
    display: grid;
    grid-template-columns: max-content max-content max-content max-content;
    gap:6%;

  }

  /*FOOTER*/
  .footer {
    height: max-content;

  }

  .company__logo {

    font-size: 1.9em;
  }

  .dados_project i {
    font-size: 2em;
  }

  .dados_project {
    align-items: center;
  }

  .project__description {
    font-size: 1em;
  }

  .copyright {
    font-size: 1em;
  }

  .project__info {
    display: grid;
    grid-template-columns: max-content max-content max-content;
    gap: 2%;
    justify-items: center;
    overflow: hidden;
    padding: 1%;
    height: 100%;
  }

  /*FINALIZAÇÃOS*/
  
}



    </style>
</head>

<body>
    <div class="header">


        <div class="info">
            <h1>MONITORAMENTO DE DADOS</h1>
            <h4>PROJETO DE SENSORES</h4>
        </div>


    </div>
    <div class="menu">
        <button class="tablink" onclick="openPage('corpo-dados', this, '#0d0f27')" id="defaultOpen">Dados</button>
        <button class="tablink" onclick="openPage('atuadores', this, 'rgb(48, 17, 7)')">Atuadores</button>
        <button class="tablink" onclick="openPage('config', this, 'rgb(9, 51, 9)')">Modo</button>
        <button class="tablink" onclick="openPage('Add', this, 'rgb(46, 15, 43)')">Downloads</button>
    </div>

    <div id="corpo-dados" class="modo-1" style="display: flex;">
        <div class="cards">
            <div class="card temperaturec">
                <h2 class="card-title-large">Temperatura°C</h2>
                <h3 class="card-title-small">
                    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
                    <span id="temperaturec">%TMC%</span>
                    <sup class="units">&deg;C</sup>

                </h3>

            </div>
            <div class="card temperaturef">
                <h2 class="card-title-large">Temperatura°F</h2>
                <h3 class="card-title-small">
                    <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
                    <span id="temperaturec">%TMF%</span>
                    <sup class="units">&deg;F</sup>

                </h3>

            </div>

            <div class="card humidity">
                <h2 class="card-title-large">Humidade</h2>
                <h3 class="card-title-small">
                    <i class="fa-solid fa-droplet" style="color:#059e8a;"> </i>
                    <span id="umidade">%HMDT%</span>
                    <sup class="units">%</sup>
                </h3>

            </div>


            <div class="card solo">
                <h2 class="card-title-large">Solo</h2>
                <h3 class="card-title-small">
                    <i class="fa-brands fa-pagelines" style="color:#059e8a;"> </i>
                    <span id="solo">%SOLO%</span>
                </h3>

            </div>

        </div>

    </div>
    <div id="config" class="modo-1" style="display: none">

        <div id="body" class="eyes-off">
            <li class="tg-list-item" id="list_b1">
                <h4 class="modo">Modo:</h4>
                <input class="tgl tgl-flip" id="btn_mode" type="checkbox" />
                <label id="btn3" class="tgl-btn" data-tg-off="Automático" data-tg-on="Manual" for="btn_mode"></label>
            </li>
            <div class="container_data" style="display: none" id="manual-m">
                <div id="servoM" class="p-control">
                    <h1 class="titulos"> Controle do Servo</h1>
                    <span id="rangeValue">0cm</span>
                    <Input class="range" type="range" name="" value="0" min="0" max="20"
                        onChange="rangeSlide(this.value)" onmousemove="rangeSlide(this.value)"></Input>
                    <button class="button success" id="btn1">Submit</button>
                </div>

                <div id="bomba" class="p-control">
                    <h1 class="titulos"> Controle da Bomba</h1>
                    <li class="tg-list-item">
                        <input class="tgl tgl-skewed" id="btn2" type="checkbox" />
                        <label id="btn2" class="tgl-btn" data-tg-off="OFF" data-tg-on="ON" for="btn2"></label>
                    </li>
                </div>
            </div>
        </div>


    </div>


    </div>

    </div>

    </div>

    </div>

    </div>

    </div>

    <div id="atuadores" class="modo-1" style="display: none;">
        <div class="dados_extras">
            <div class="card servo">
                <h2 class="card-title-large">Braço</h2>
                <h3 class="card-title-small">
                    <i class="fa-solid fa-robot" style="color:#059e8a;"> </i>
                    <span id="servo">%SERVO%</span>
                </h3>
            </div>
            <div class="card bomba">
                <h2 class="card-title-large">Bomba</h2>
                <h3 class="card-title-small">
                    <i class="fa-solid fa-faucet-drip" style="color:#059e8a;"> </i>
                    <span id="bomba">%BOMBA%</span>
                </h3>
            </div>
        </div>
    </div>

    <div id="Add" class="modo-1" style="display: none;">
        <div class="baixar-dados">
            <span class="titulo-medio">BAIXAR DADOS COMO:</span>
            <div class="grid-btn">
                <button class='exportar' style="--clr:#39FF14"><span>CSV</span><i></i></button>

                <button class='exportar' style="--clr:#FF44CC"><span>PDF</span><i></i></button>

                <button class='exportar' style="--clr:#0FF0FC"><span>ZIP</span><i></i></button>

                <button class='exportar' style="--clr:#8A2BE2"><span>RAR</span><i></i></button>
            </div>
        </div>



    </div>

    <div class="footer">

        <div class="project__info">
            <div class="dados_project">
                <h2 class="company__logo">Aline Mariana</h2>
                <ul class="footer__list">
                    <li class="footer__list-item">
                        <a href="#" class="footer__list-link">
                            <i class="fa-brands fa-github"></i>
                        </a>
                    </li>
                    <li class="footer__list-item">
                        <a href="https://www.linkedin.com/in/aline-mariana-b83575184/" class="footer__list-link">
                            <i class="fa-brands fa-linkedin"></i>
                        </a>
                    </li>
                </ul>
            </div>
            <p class="project__description">
                Projeto para Monitoramento da humidade e temperatura do solo.
            </p>
            <span class="copyright">&copy;2023 People name. All rights reserved.</span>
        </div>
    </div>

    </div>
</body>
<script>

    const themeSwitch = document.getElementById("btn_mode");

    const body = document.getElementById("body");
    const manual = document.getElementById("manual-m");

    themeSwitch.addEventListener("click", () => {


        if (body.className === "eyes-off") {
            manual.style.display = "grid";
            body.className = "eyes-on";
        } else if (body.className === "eyes-on") {
            manual.style.display = "none";
            body.className = "eyes-off";

        }
    });


    const casa = document.getElementById("corpo-dados");
    const config = document.getElementById("config");
    const contact = document.getElementById("atuadores");
    const about = document.getElementById("Add");

    themeSwitch.addEventListener("click", () => {
        let currentBodyClass = casa.className;

        if (casa.className === "modo-1") {
            casa.className = "modo-2";
            config.className = "modo-2";
            contact.className = "modo-2";
            about.className = "modo-2";
        } else if (casa.className === "modo-2") {

            casa.className = "modo-1";
            config.className = "modo-1";
            contact.className = "modo-1";
            about.className = "modo-1";
        }
    });

    function openPage(pageName, elmnt, color) {

        var i, dados, tablinks;
        dados = document.getElementsByClassName("modo-1");

        if (dados.length < 4) {
            dados = document.getElementsByClassName("modo-2");
        }

        for (i = 0; i < dados.length; i++) {
            dados[i].style.display = "none";
        }
        tablinks = document.getElementsByClassName("tablink");
        for (i = 0; i < tablinks.length; i++) {
            tablinks[i].style.backgroundColor = "";
        }
        document.getElementById(pageName).style.display = "flex";
        elmnt.style.backgroundColor = color;
    }

    document.getElementById("defaultOpen").click();

    function rangeSlide(value) {
        document.getElementById('rangeValue').innerHTML = value + "cm";
    }

    var animateButton = function (e) {

        e.preventDefault;
        //reset animation
        e.target.classList.remove('animate');

        e.target.classList.add('animate');

        e.target.classList.add('animate');
        setTimeout(function () {
            e.target.classList.remove('animate');
        }, 6000);
    };

    var classname = document.getElementsByClassName("button");

    for (var i = 0; i < classname.length; i++) {
        classname[i].addEventListener('click', animateButton, false);
    }

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
              document.getElementById("temperaturef").innerHTML = this.responseText;
            }
          };
          xhttp.open("GET", "/temperaturef", true);
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
        setInterval(function ( ) {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById("solo").innerHTML = this.responseText;
            }
          };
          xhttp.open("GET", "/solo", true);
          xhttp.send();
        }, 10000) ;

    setInterval(function ( ) {
          var xhttp = new XMLHttpRequest();
          xhttp.onreadystatechange = function() {
            if (this.readyState == 4 && this.status == 200) {
              document.getElementById("bomba").innerHTML = this.responseText;
            }
          };
          xhttp.open("GET", "/bomba", true);
          xhttp.send();
          
        }, 10000) ;
</script>

</html>

)rawliteral";

// Replaces placeholder with DS18B20 values
String processor(const String &var)
{
  // Serial.println(var);
  if (var == "TMC")
  {
    return temperatureC;
  }
  else if (var == "TMF")
  {
    return temperatureF;
  }
  else if (var == "BOMBA")
  {
    return estado_bomba;
  }

  else if (var == "HMDT")
  {
    return humidity;
  }
  else if (var == "SERVO")
  {
    return estado_servo;
  }
  else if (var == "SOLO")
  {
    return solo;
  }

  return String();
}

// CONFIG DE IP
IPAddress local_IP(192, 168, 141, 31);
IPAddress gateway(192, 168, 141, 0);
IPAddress subnet(255, 255, 255, 0);


//FUNCOES DO MODO


void modo_Manual()
{
int a = 0;

}

void modo_Automatico()
{
 int value = analogRead(AOUT_PIN);
  float percent = Percent_Moisture(value);
  if (percent < regular)
  {
    estado_servo = writeServo(1);
    // int pos = 0;
    // Armar o Braço
    for (pos = 0; pos <= 180; pos += 1)
    {

      myservo.write(pos);
      delay(15);
    }
    humidity = readDSHumidity();
    // Ligar o Motor
    int limite = 0;
    // myservo.write(pos);
    Serial.println("Irrigando...");
    while (limite != 1)
    {
      int value = analogRead(AOUT_PIN);
      float percent = Percent_Moisture(value);
      humidity = readDSHumidity();
      // Serial.println("Percentual:");
      // Serial.println(percent,2);
      // Serial.print("Moisture value: ");
      // Serial.println(value);

      delay(5000);
      if (percent > otimo)
      {
        limite = 1;
      }
    }
    // Desligar Motor

    Serial.println("Finalizando a Irrigação");
    delay(5000);
    estado_servo = writeServo(2);
    // Voltar o Braço
    for (pos = 180; pos >= 0; pos -= 1)
    {                     // goes from 180 degrees to 0 degrees
      myservo.write(pos); // tell servo to go to position in variable 'pos'
      delay(15);          // waits 15ms for the servo to reach the position
    }

    // myservo.write(0);
    Serial.println("Irrigação Concluida");
    Serial.println("-------------------------");
    estado_servo = writeServo(3);
  }
}
void setup()
{

  Serial.begin(115200);
  Serial.println();

  // INCIANDO AS PORTAS

  sensors.begin();

  temperatureC = readDSTemperatureC();

  // Humidity
  humidity = readDSHumidity();

  // Servo
  estado_servo = writeServo(3);

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
  server.on("/temperaturec", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", temperatureC.c_str()); });
  server.on("/umidade", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", humidity.c_str()); });
  server.on("/servo", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", estado_servo.c_str()); });
  server.on("/solo", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", solo.c_str()); });
  server.on("/temperaturef", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", temperatureF.c_str()); });
  server.on("/bomba", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send_P(200, "text/plain", estado_bomba.c_str()); });
  // Start server
  server.begin();

  myservo.attach(ServoPin);
}

void loop()
{

  // Leitura Inicial
  Serial.println("----------------------------");
  temperatureC = readDSTemperatureC();
  temperatureF = readDSTemperatureF();
  humidity = readDSHumidity();
  estado_servo = writeServo(3);
  estado_bomba = writeBomba(1);

  delay(100);
}
