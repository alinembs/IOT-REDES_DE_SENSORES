#include "config_server.h"
#include "config_wifi.h"

void init_App()
{
     Serial.println("Inicializando Aplicação");
    delay(1000);
    // TIME

    // INCIANDO AS PORTAS
    Serial.println("Inicializando as Portas /PIN");
    myservo.attach(ServoPin);
    pinMode(rele, OUTPUT);
    //sensors.begin();

    // TEMPERATURE
    temperatureC = readDSTemperatureC();
    temperatureF = readDSTemperatureF();

    // Humidity

    humidity = readDSHumidity();

    // Servo Bomb
    Bomba_Agua("OFF");

    estado_servo = writeSERVO(0);
    // Incia o Wifi
    init_Wifi_AP();
    // Incia o Cartão de Memoria
    initSDCard();
    // Inicia o SPIFFS
    initSPIFFS();
    // Inica o Server
    init_Server();
}