//
//  WifiModule.h
//  esp8266
//
//  Created by <author> on 22/02/2018.
//
//

#ifndef WifiModule_H
#define WifiModule_H

#include <stdio.h>
#include <ESP8266WiFi.h>      //ESP8266 Core WiFi Library (you most likely already have this in your sketch)
#include <DNSServer.h>        //Serveur DNS local utilisé pour rediriger toutes les requêtes vers le portail de configuration
#include <ESP8266WebServer.h> //Serveur Web local utilisé pour servir le portail de configuration
#include <WiFiManager.h>      //https://github.com/tzapu/WiFiManager WiFi Configuration Magic
#include "ConfigModuleJson.h"
// #include <ElegantOTA.h>

/**
 * Gestion de la connexion au Wifi et configuration des identifiants Wifi en créant un point d'accès.
 */
class WifiModule
{
private:
    String deviceName;
    WiFiManager wifiManager;

    WiFiManagerParameter parameterEnableTime = WiFiManagerParameter("enable_time", "Enable Time (HH:MM)", "", 6);
    WiFiManagerParameter parameterDisableTime = WiFiManagerParameter("disable_time", "Disable Time (HH:MM)", "", 6);

public:
    WifiModule(String _deviceName);
    ~WifiModule();
    void setup(void (*configModeCallback)(WiFiManager *myWiFiManager), void (*saveConfig)(void));
    bool isConnected();
    bool connect();
    void reset();
    SimpleTime getEnableTime();
    SimpleTime getDisableTime();

protected:
};

#endif /* WifiModule_h */
