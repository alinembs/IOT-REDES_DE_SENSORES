#define TRIG 25 //Module pins
#define ECHO 26 

float leiturasimple();
float calcularDist();
void InicializarPinos(byte trig, byte echo);

float distancia;

void setup()
{

Serial.begin(9600);
InicializarPinos(TRIG,ECHO);



}

void loop()
{

distancia = calcularDist();

 if (distancia <= 18){
    Serial.println("Out of range");
  }
  else {
    delay(1000);
    Serial.print("Distância: " ); 
    Serial.print(distancia,0);
    Serial.print("cm");
    Serial.print("/n");  
    
  }
  
}


void InicializarPinos(byte trig, byte echo){

#define divisor  58.82
#define intervalo_medido  5
#define qtMedidas 20

pinMode(trig, OUTPUT);
pinMode(echo, INPUT);

digitalWrite(trig,LOW);
delayMicroseconds(500);

}

float calcularDist(){

  float leituraSum = 0;
  float resultado = 0 ;

for (int index = 0 ; index< qtMedidas; index++ ){

  delay(intervalo_medido);
  leituraSum += leituraSimples();

} 

  resultado = (float) leituraSum/qtMedidas;
  resultado = resultado + 2.2;
return resultado;

}

float leituraSimples(){

long duracao = 0;
float resultado = 0;
digitalWrite(ECHO, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG, LOW);

duracao = pulseIn(ECHO, HIGH);
resultado = (float) duracao / divisor;


return resultado;
}
