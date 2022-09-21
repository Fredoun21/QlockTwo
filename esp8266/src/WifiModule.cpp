//
//  WifiModule.cpp
//  esp8266
//
//  Created by <author> on 22/02/2018.
//
//

#include "WifiModule.h"

WifiModule::WifiModule(String _deviceName) : deviceName(_deviceName) {}

WifiModule::~WifiModule() {}

/**
 * Setup WifiManager
 * @param configModeCallback Est appelé lorsque WiFiManager passe en mode de configuration.
 * @param saveConfig Est appelé lorsque WifiManager lorsque des paramètres personnalisés ont été définis ET qu'une connexion a été établie.
 */
void WifiModule::setup(void (*configModeCallback)(WiFiManager *myWiFiManager), void (*saveConfig)(void))
{
    wifiManager.setAPCallback(configModeCallback);
    wifiManager.setSaveConfigCallback(saveConfig);

    // délai d'attente en cas de panne de courant, réessayez de vous connecter après 180 s
    wifiManager.setConfigPortalTimeout(180);

    wifiManager.addParameter(&parameterDisableTime);
    wifiManager.addParameter(&parameterEnableTime);
    wifiManager.addParameter(&parameterLedColor);
    wifiManager.addParameter(&parameterBrightness);
    // void (WifiModule::*saveConfig)(WiFiManager *myWiFiManager);
    // saveConfig = &WifiModule::configModeCallback;
}

/**
 * Vérifiez si le Wifi est connecté.
 * @return true on connected
 */
bool WifiModule::isConnected()
{
    return WiFi.status() == WL_CONNECTED;
}

/**
 * Essayez de vous connecter au Wifi préenregistré, sinon passez en mode AP. Redémarre ESP en cas d'échec.
 * @return Success
 */
bool WifiModule::connect()
{
    Serial.println("WifiModule: Connection en cours \n");
    if (!wifiManager.autoConnect(this->deviceName.c_str()))
    {
        Serial.println("Échec de la connexion et dépassement du délai d'attente");
        // réinitialisez et réessayez, ou mettez-le peut-être en veille profonde
        ESP.reset();
        delay(1000);

        return false;
    }

    Serial.println("Wifi connecté...yeey :) \n");

    return true;
}

/**
 * Supprimez les informations d'identification du réseau Wi-Fi enregistrées.
 */
void WifiModule::reset()
{
    Serial.println("WifiModule: Reset.");
    wifiManager.resetSettings();
}

/**
 * Renvoie EnableTime à partir des paramètres définis.
 * @return EnableTime
 */
SimpleTime WifiModule::getEnableTime()
{
    return SimpleTime::parse(parameterEnableTime.getValue());
}

/**
 * Renvoie DisableTime à partir des paramètres définis.
 * @return DisableTime
 */
SimpleTime WifiModule::getDisableTime()
{
    return SimpleTime::parse(parameterDisableTime.getValue());
}

/**
 * Renvoie LedColor à partir des paramètres définis.
 * @return LedColor
 */
String WifiModule::getLedColor()
{
    return parameterLedColor.getValue();
};

/**
 * Renvoie Brightness à partir des paramètres définis.
 * @return Brightness
 */
String WifiModule::getBrightness()
{
    return parameterBrightness.getValue();
};