#include <Arduino.h>
#include <WiFi.h>
#include <ESPAsyncWebServer.h>



// Replace with your network credentials
const char* ssid = "Selma Maria_Fibra";
const char* password = "88599380";

// Create AsyncWebServer object on port 80
AsyncWebServer server(80);


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

h4{
  text-decoration:solid
}

.header{
  position:relative;
  overflow:hidden;
  display:flex;
  flex-wrap: wrap;
  justify-content: center;
  align-items: flex-start;
  align-content: flex-start;
  height:50vw;
  min-height:200px;
  max-height:350px;
  min-width:300px;
  color: #cfc0c0;
}
.header:after{
  content:"";
  width:100%;
  height:100%;
  position:absolute;
  bottom:0;
  left:0;
  z-index:-1;
 background: linear-gradient(to bottom, rgba(0,0,0,0.12) 40%,rgba(27,32,48,1) 100%);
}
.header:before{
  content:"";
  width:100%;
  height:200%;
  position:absolute;
  top:0;
  left:0;
    -webkit-backface-visibility: hidden;
    -webkit-transform: translateZ(0); backface-visibility: hidden;
  scale:(1.0, 1.0);
  transform: translateZ(0);
  background:#1B2030 url(https://images.unsplash.com/photo-1586771107445-d3ca888129ff?ixlib=rb-4.0.3&ixid=MnwxMjA3fDB8MHxzZWFyY2h8MTF8fGFncmljdWx0dXJlfGVufDB8fDB8fA%3D%3D&auto=format&fit=crop&w=1170&q=80) 50% 0 no-repeat;
  background-size:100%;
  background-attachment:fixed;
  animation: grow 360s  linear 10ms infinite;
  transition:all 0.4s ease-in-out;
  z-index:-2
}
.header a{
  color: #b4afaf;
}
.sides{
   position:center;
}
.sides, .info{
  flex: 0 0 auto;
  width:50%

}
.info{
  width:100%;
  padding:15% 10% 0% 0%;
  text-align:center;
  text-shadow:0 2px 3px rgba(0,0,0,0.2)
}

.info h4, .meta{
  font-size:0.7em
}

@keyframes grow{
  0% { transform: scale(1) translateY(0px)}
  50% { transform: scale(1.2) translateY(-400px)}
}

/* */
.dados{
  position:relative;
  overflow:hidden;
  display:flex;
  flex-wrap: wrap;
  justify-content: center;
  align-items: flex-start;
  align-content: flex-start;
  height: 100%;
  min-height:200px;
  width:auto;
  background-color: #12131c;
  min-width:300px;
 
}

.cards {
  display: grid;
  grid-template-columns:1fr 1fr 1fr 1fr;
  gap: 50px;
  padding-top: 4%;
  }

.card {
    background: #262525;
    color: #fff;
    padding: 1.5rem;
    overflow: hidden;
    border-radius: 0.8rem;
    width: 50vw;
    height: 50vw;
    min-height:20px;
    max-height:250px;
    min-width:20px;
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
        background: radial-gradient(
          at 20% 128%,
          #a826b4 20%,
          #942a4a 60%,
          #971133 90%
        );
        }
.card.solo:hover {
        background-color:#26790d;
        }
.card.temperature:hover {
        background-color: #ff6207;
        }
        
/* */
i {
  font-size: 2.4rem;
}


.footer{
  position:relative;
  overflow:hidden;
  display:flex;
  flex-wrap: wrap;
  justify-content: center;
  align-items: center;
  align-content: flex-start;
  height:50vw;
  min-height:100px;
  max-height:200px;
  background-color: #151823;
  min-width:300px;
  
}
.footer__container {
  color: #666873;
  max-width: 80rem;
  margin: center;
  
}

.footer__top {
  display: grid;
  grid-template-rows: 
  repeat(auto-fit, minmax(20rem, 1fr));
  row-gap: 1rem;
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

.company__info {
  grid-column: 1/-1;
  display: flex;
  flex-direction: column;
  align-items: center;
  padding-top: 2%;

}
.company__logo {
  color: #fff;
}
.company__info ul {
  flex-direction: row;
}

.infodados{
  display: grid;
grid-template-columns: 1fr 1fr;

}

@media (max-width: 400px){

  .header{
 height:25vw;
}

.info h4, .meta{
  font-size:0.9em
}
.info h1, .meta{
  font-size:1.5em
}

.cards {
  display: grid;
  grid-template-columns: auto auto;
  padding-top: 22%;

  }

.card {
    padding: 2.5rem;
    border-radius: 0.8rem;
    width: 14vw;
    height: 14vw; 
    }
   
.card-title-large {

      font-size: 1.3rem;
      text-align: right;
      letter-spacing: 0.5rem;
      }
.card-title-small {
      display: grid;
      grid-template-columns: 1fr 1fr 1fr;
      font-size: 0.75rem;
      
      }
    
/* */
i {
  font-size: 0.8rem;
}


.footer{ 
  height:100%;

  
}
.footer__container {
  margin: center;
  
}

.footer__top {
  display: grid;
  grid-template-rows: 
  repeat(auto-fit, minmax(20rem, 1fr));
  row-gap: 1rem;
}
.footer__title {
  font-size: 1rem;
  font-weight:200;
  margin-bottom: 1rem;
}
.footer__list {
  display: flex;
  flex-direction: column;
  gap: 2rem;
  
}

.company__info {
  grid-column: 1/-1;
  display: flex;
  flex-direction: column;
  align-items:center;
  padding-top: 2%;

}
.company__logo {
  color: #fff;
  font-size: 1.2rem;
}
.company__info ul {
  flex-direction: row;
}
.company__description {
  font-size: 0.74rem;
  text-align: center;
}
.copyright{
  font-size: 0.9rem;

}

.infodados{
  display: grid;
grid-template-columns: 1fr 1fr;

}
}

@media (min-width:601px) and (max-width: 800px) {

.info h1, .meta{
  font-size:1.5em;
}
 
.cards {
  display: grid;
  grid-template-columns: auto auto;
  padding-top: 4%;

  }

.card {
    padding: 2.5rem;
   border-radius: 0.8rem;
    width: 14vw;
    height: 14vw;
    
    }
   
.card-title-large {
      
      font-size: 1.9rem;
      text-align: right;
      letter-spacing: 0.5rem;
  
      }
.card-title-small {
      display: grid;
      grid-template-columns: 1fr 1fr 1fr;
      font-size: 1.1rem;
      
      }
     


/* */
i {
  font-size: 1rem;
}

.company__info {
  grid-column: 1/-1;
  display: flex;
  flex-direction: column;
  align-items:center;
  padding-top: 2%;

}
.company__logo {
 
  font-size: 1.3rem;
}

.company__description {
  font-size: 0.9rem;
  text-align: center;
}
.copyright{
  font-size: 0.9rem;

}

}

@media (min-width:401px) and (max-width: 600px) {

.cards {
  display: grid;
  grid-template-columns: auto auto;
  padding-top: 10%;

  }

.card {
    
    padding: 2.5rem;
    overflow: hidden;
    border-radius: 0.8rem;
    width: 14vw;
    height: 14vw;
    
    }
   
.card-title-large {
    
      font-size: 1.7rem;
      text-align: right;
      letter-spacing: 0.5rem;
    
      }
.card-title-small {
      display: grid;
      grid-template-columns: 1fr 1fr 1fr;
      font-size: 0.9rem;
      
      }
     
/* */
i {
  font-size: 0.9rem;
}


.footer{
  position:relative;
  overflow:hidden;
  display:flex;
  flex-wrap: wrap;
  justify-content: center;
  align-items: center;
  align-content: flex-start;
  height:100%;
  background-color: #151823;
  
}
.footer__container {
  color: #666873;
  margin: center;
  
}

.company__info {
  grid-column: 1/-1;
  display: flex;
  flex-direction: column;
  align-items:center;
  padding-top: 2%;

}
.company__logo {
  color: #fff;
  font-size: 1.3rem;
}
.company__info ul {
  flex-direction: row;
}
.company__description {
  font-size: 0.80rem;
  text-align: center;
}
.copyright{
  font-size: 0.9rem;

}

.infodados{
  display: grid;
grid-template-columns: 1fr 1fr;

}
}
@media (min-width:801px) and (max-width: 1000px) {

.cards {
  display: grid;
  grid-template-columns: auto auto;
  padding-top: 3%;

  }

.card {
    padding: 2.5rem;
    overflow: hidden;
    border-radius: 0.8rem;
    width: 10vw;
    height: 10vw;
    
    }
   
.card-title-large {
     
      font-size: 1.9rem;
      text-align: right;
      letter-spacing: 0.5rem;
     
      }
.card-title-small {
      display: grid;
      grid-template-columns: 1fr 1fr 1fr;
      font-size: 1.1rem;
      
      }
/* */
i {
  font-size: 1rem;
}

.footer__container {
  color: #666873;
  margin: center;
  
}

.company__info {
  grid-column: 1/-1;
  display: flex;
  flex-direction: column;
  align-items:center;
  padding-top: 2%;

}
.company__logo {
  color: #fff;
  font-size: 1.3rem;
}
.company__info ul {
  flex-direction: row;
}
.company__description {
  font-size: 0.9rem;
  text-align: center;
}
.copyright{
  font-size: 0.9rem;

}
.info h4, .meta{
  font-size:1em
}
.info h1, .meta{
  font-size:1.5em
}

}
@media (min-width:1001px) and (max-width: 1300px) {
  
.cards {
  display: grid;
  grid-template-rows:1fr 1fr 1fr 1fr;
  padding-top: 5%;

  }

.card {
    padding: 2.5rem;
    overflow: hidden;
    border-radius: 0.8rem;
    width: 13vw;
    height: 13vw;
    
    }
   
.card-title-large {
     
      font-size: 2rem;
      text-align: right;
      letter-spacing: 0.5rem;
      }
.card-title-small {
      display: grid;
      grid-template-columns: 1fr 1fr 1fr;
      font-size: 1.3rem;
      
      }
     
/* */
i {
  font-size: 1.5rem;
}


.footer__container {
  color: #666873;
  margin: center;
  
}

.company__info {
  grid-column: 1/-1;
  display: flex;
  flex-direction: column;
  align-items:center;
  padding-top: 2%;

}
.company__logo {
  color: #fff;
  font-size: 1.5rem;
}
.company__info ul {
  flex-direction: row;
}
.company__description {
  font-size: 1rem;
  text-align: center;
}
.copyright{
  font-size: 1rem;

}
.info h4, .meta{
  font-size:1.2em
}
.info h1, .meta{
  font-size:1.8em
}

}
    </style>
    </head>
    <body style="display: flex; flex-direction: column; justify-content: space-between; height: 100vh">
  <div class="header">
    <div class="sides">
      <div class="info">
         <h1>MONITORAMENTO DE DADOS</h1>
            <h4>PROJETO DE SENSORES</h4>
      </div>
     </div>
  </div>
  <div class = "dados">
   
    <div class="cards">
        <div class="card humidity">
            <h2 class="card-title-large">Humidade</h2>
            <h3 class="card-title-small">
                <i class="fa-solid fa-droplet" style="color:#059e8a;"> </i>
                  <span id="umidade">%HMDT%</span>
               <sup class="units">%</sup>
                </h3>

            
        </div>
        <div class="card temperature">
            <h2 class="card-title-large">Temperatura</h2>
            <h3 class="card-title-small">
                <i class="fas fa-thermometer-half" style="color:#059e8a;"></i>
                  <span id="temperaturec">%TMPC%</span>
                   <sup class="units">&deg;C</sup>

               </h3>
        
        </div>

        <div class="card solo">
            <h2 class="card-title-large">Solo</h2>
            <h3 class="card-title-small">
                <i class="fa-brands fa-pagelines" style="color:#059e8a;"> </i>
                  <span id="solo">%SOLO%</span>
               </h3>
                
        </div>
        <div class="card servo">
            <h2 class="card-title-large">Braço</h2>
            <h3 class="card-title-small">
               <i class="fa-solid fa-robot" style="color:#059e8a;"> </i>
                <span id="servo">%SERVO%</span>
              </h3> 
        </div>
    </div>
        
  </div>
  <div class="footer">
    <div class="footer__container">
      <div class="footer__top">
          <div class="company__info">
             <div class="infodados">
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
              <p class="company__description">
                  Projeto para Monitoramento da humidade e temperatura do solo, para atuação de um
                  braço robótico acoplado com uma magueira para sua irrigação.
              </p>
              <span class="copyright">&copy;2023 People name. All rights reserved.</span>
          </div>
      </div>
  </div>
  </div>
  </div>
</body>
<script>

        const bodyEl = document.body
        const activeColor = document.querySelector('.card')
        const activeTheme = localStorage.getItem('theme')
        
        if(activeTheme)
        {
        bodyEl.classList.add('bom')
        bodyEl.classList.add('pessimo')
        bodyEl.classList.add('razoavel')
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
              if(this.responseText == 'Positivo')
              {
                localStorage.setItem('theme','bom')
              }
              if(this.responseText == 'Regular')
              {
                localStorage.setItem('theme','razoavel')
              }
              else if(this.responseText == 'Pessimo')
              {
                localStorage.setItem('theme','pessimo')
              }
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
        
</script>
</html>

)rawliteral";

// Replaces placeholder with DS18B20 values

// Set your Gateway IP address
IPAddress local_IP(192, 168, 100, 186);
IPAddress gateway(192, 168, 100, 1);
IPAddress subnet(255, 255, 255, 0);


void setup(){

  // Serial port for debugging purposes
  Serial.begin(115200);
  Serial.println();
  
  // Start up the DS18B20 library

  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet)) {
    Serial.println("STA Failed to configure");
  }
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
    request->send_P(200, "text/html", index_html);
  });
  // Start server
  server.begin();

  
}
 
void loop(){

  //Leitura Inicial 
    Serial.println("----------------------------");
    delay(100);
  }  