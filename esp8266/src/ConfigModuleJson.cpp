//
// Enregistrez et chargez la configuration vers/depuis le SPIFF de l'ESP.
//

#include "ConfigModuleJson.h"

ConfigModule::ConfigModule(String _configFilePath) : configFilePath(_configFilePath){};

void ConfigModule::setup()
{
    if (!LittleFS.begin())
    {
        Serial.println("Impossible de monter le système de fichiers");
        return;
    }

    //LittleFS.format();
}

/**
 * Sérialisez la configuration en JSON et enregistrez-la en SPIFF.
 * @param config
 * @return true on success
 */
bool ConfigModule::saveConfig(const Config &config)
{
    Serial.println("enregistrement de la configuration");
    const size_t capacity = JSON_OBJECT_SIZE(10);
    DynamicJsonDocument doc(capacity);

    copyToJsonConfig(config, doc);

    File configFile = LittleFS.open(configFilePath, "w");
    if (!configFile)
    {
        Serial.println("impossible d'ouvrir le fichier de configuration pour l'écriture");
        return false;
    }

    serializeJson(doc, Serial);
    serializeJson(doc, configFile);

    configFile.close();
    // end save

    return true;
}

/**
 * Chargez la configuration JSON à partir de SPIFF et désérialisez-la.
 * @return En cas de succès : configuration enregistrée ; En cas d'échec : configuration vide
 */
const Config ConfigModule::loadConfig()
{
    if (LittleFS.exists(configFilePath))
    {
        // fichier existe, lecture et chargement
        Serial.println("lecture du fichier de configuration");
        File configFile = LittleFS.open(configFilePath, "r");
        if (configFile)
        {
            Serial.println("fichier de configuration ouvert");

            const size_t capacity = JSON_OBJECT_SIZE(10) + 90;
            DynamicJsonDocument doc(capacity);
            DeserializationError error = deserializeJson(doc, configFile);

            if (error)
            {
                Serial.println("échec du chargement de la configuration json");
                Serial.println(error.c_str());
            }

            configFile.close();
            return parseJsonConfig(doc);
        }
    }

    return Config();
}

/**
 * Remplissez les valeurs de configuration de JsonObject à Config struct.
 * @param jsonObject
 * @return Filled Config Object.
 */
Config ConfigModule::parseJsonConfig(const JsonDocument &jsonObject)
{
    Config config;

    config.enableTime = SimpleTime::parse(jsonObject[ENABLE_TIME_ID]);
    config.disableTime = SimpleTime::parse(jsonObject[DISABLE_TIME_ID]);
    config.setLedColor = jsonObject[SET_LED_COLOR_ID];
    config.brightnessCorrection = jsonObject[BRIGHTNESS_CORRECTION_ID];

    return config;
}

/**
 * Copier les valeurs de configuration dans JsonObject
 * @param json To be filled JsonObject, by reference
 * @param config Source Config
 */
void ConfigModule::copyToJsonConfig(const Config &config, JsonDocument &doc)
{
    doc[DISABLE_TIME_ID] = config.disableTime.toString();
    doc[ENABLE_TIME_ID] = config.enableTime.toString();
    doc[SET_LED_COLOR_ID] = config.setLedColor;
    doc[BRIGHTNESS_CORRECTION_ID] = config.brightnessCorrection;
}
