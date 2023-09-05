#include "config_server.h"
#include "config_wifi.h"

void init_App()
{
    Serial.println("Inicializando Aplicação");
    delay(1000);
    // Iniciar o RTC - Data e Hora
    init_RTC();
    // INCIANDO AS PORTAS
    Serial.println("Inicializando as Portas /PIN");

    // TEMPERATURE
    sensors.begin();
    // Humidity
    preferences.begin("my-app", false);
    bool data_modulo = preferences.getBool("data_modulo", false);

    if (data_modulo == false)
    {
        // Inicia o Relogio se precisar 

        rtc.adjust(DateTime(__DATE__, __TIME__));
        Serial.println("Data e hora definidas!");
        data_modulo = true;
    }
    preferences.putBool("data_modulo", data_modulo);
    preferences.end();

    // Incia o Wifi
    // init_Wifi_AP();
    init_Wifi_NM();
    // Incia o Cartão de Memoria
    initSDCard();
    // Inicia o SPIFFS
    initSPIFFS();
    // Inica o Server
    init_Server();
}
