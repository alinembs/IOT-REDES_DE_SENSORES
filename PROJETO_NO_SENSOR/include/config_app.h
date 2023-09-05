
// INCLUDES DOS SENSORES
#include "temperature.h"
#include "humidity.h"
// INCLUDES DOS ATUADORES
#include "fluxo_de_agua.h"
// INCLUDES DAS REQUIÇOES DO BRAÇO ROBOTICO + BOMBA D'GUA
#include "requisicoes.h"


//Funcao para Irrigar plantas de acordo com sua humidade e temperatura e controle de vazão
void IRRIGACAO()
{
 //FUNCAO Mais importante do Projeto
}



void mode_app(String mode)
{
  if ((mode == " ") || (mode == "0"))
  {
    Serial.println("-----------MODO AUTOMÁTICO ----------");
  }
  else if (mode == "1")
  {
    Serial.println("-----------MODO MANUAL ----------");
  }
  else
  {
    Serial.println("-----------ERRO DE MODO ----------");
  }

}

