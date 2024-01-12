

#define limite 1295 
#define min_moisture 4096
#define max_moisture 2800

#define otimo 75
#define regular 60
#define pessimo 50


#define wet 4095
#define dry 0

#define AOUT_PIN 36// ESP32 pin GIOP36 (ADC0) that connects to AOUT pin of moisture sensor

String solo = " ";
String humidity = " ";

// FUNÇOES

// float Percent_Moisture(int value)
// {
//   int valor = 100 - ((value - max_moisture) * 100) / limite;
//   if (valor < -1)
//   {
//     valor = 0;
//   }
//   if (valor > 100)
//   {
//     valor = 100;
//   }
//   return valor;
// }


// String writeSolo(float percent)
// {
//   String solo = "";

//   if (percent > 75)
//   {
//     solo = "Positivo";
//     // Serial.println(solo);
//   }
//   if ((percent < 75) & (percent > 50))
//   {
//     solo = "Regular";
//     // Serial.println(solo);
//   }
//   if (percent < 50)
//   {
//     solo = "Crítico";
//     // Serial.println(solo);
//   }

//   if (solo == "")
//   {
//     Serial.println("Failed to find ground state");
//     return "--";
//   }
//   else
//   {
//    // Serial.print("Estado do Solo: ");
//    // Serial.println(solo);
//   }
//   return solo;
// }

// String readDSHumidity()
// {
//   //int value_media = 0;
//  /* float media;
//   int indice;
//   for (indice= 0;indice<5;++indice)
//   {
//     delay(500);
//     int value = analogRead(AOUT_PIN);
//     value_media =  value_media + value;
//     Serial.println("Valores:",value_media);
//   }
// */
//   //media = value_media/5;
//   //Serial.print("Humidity: ");
//   //Serial.println(media);
//    int value = analogRead(AOUT_PIN);
//  // float percent = Percent_Moisture(media);
//  float percent = Percent_Moisture(value);
//   solo = writeSolo(percent);
//   if ((percent < -1) & (percent > 100))
//   {
//    // Serial.println("Failed to read from Humidity sensor");
//     return "--";
//   }
//   else
//   {
//     //Serial.print("Humidity %: ");
//     // percent = 0;
//     //Serial.println(percent);
//   }
//   humidity = String(percent);
//   return String(percent);
// }


//Funcao para retornar a porcentagem da Umidade
String onSensorChange()  {
  float sensor;
  sensor = analogRead(AOUT_PIN);
  sensor =  map(sensor,dry,wet,0,100);

  humidity = String(sensor);
  return  String(sensor);
}