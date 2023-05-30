#include "config_server.h"
#include "config_wifi.h"


void init_App()
{
    // Incia o Wifi
    init_Wifi_AP();
    // Incia o Cartão de Memoria
    initSDCard();
    // Inicia o SPIFFS
    initSPIFFS();
    // Inica o Server
    init_Server();
}