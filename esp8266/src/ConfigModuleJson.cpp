//
// Enregistrez et chargez la configuration vers/depuis le SPIFF de l'ESP.
//

#include "ConfigModuleJson.h"

ConfigModuleJson::ConfigModuleJson(String _configFilePath) : configFilePath(_configFilePath) {}

ConfigModuleJson::~ConfigModuleJson() {}

/**
 * Démarre le système de fichier LittleFS.
 *
 * @return true on success
 */
void ConfigModuleJson::setup()
{
    if (!LittleFS.begin())
    {
        Serial.println("Impossible de monter le système de fichiers");
        return;
    }

    // Get all information about SPIFFS
    FSInfo fsInfo;
    LittleFS.info(fsInfo);
    Serial.println("File system info");

    // Taille de la zone de fichier
    Serial.print("Total space:      ");
    Serial.print(fsInfo.totalBytes);
    Serial.println("byte");

    // Espace total utilise
    Serial.print("Total space used: ");
    Serial.print(fsInfo.usedBytes);
    Serial.println("byte");

    // Taille d un bloc et page
    Serial.print("Block size:       ");
    Serial.print(fsInfo.blockSize);
    Serial.println("byte");

    Serial.print("Page size:        ");
    Serial.print(fsInfo.totalBytes);
    Serial.println("byte");

    Serial.print("Max open files:   ");
    Serial.println(fsInfo.maxOpenFiles);

    // Taille max. d un chemin
    Serial.print("Max path lenght:  ");
    Serial.println(fsInfo.maxPathLength);

    // Open folder
    Dir dir = LittleFS.openDir("/");
    // Affiche le contenu du dossier racine | Print dir the content
    while (dir.next())
    {
        // recupere le nom du fichier
        Serial.print(dir.fileName());
        Serial.print(" - ");
        // et sa taille
        if (dir.fileSize())
        {
            File f = dir.openFile("r");

            // lecture et affichage du fichier config.json
            Serial.println(f.size());
            while (f.available())
            {
                Serial.write(f.read());
            }
            Serial.println();
            f.close();
            Serial.println();
        }
        else
        {
            Serial.println("file is empty");
        }
    }
    // LittleFS.format();
}

/**
 * Sérialisez la configuration en JSON et enregistrez-la en SPIFF.
 * @param config
 * @return true on success
 */
bool ConfigModuleJson::saveConfig(const Config &config)
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

    configFile.close(); // end save

    return true;
}

/**
 * Chargez la configuration JSON à partir de SPIFF et désérialisez-la.
 * @return En cas de succès : configuration enregistrée ; En cas d'échec : configuration vide
 */
const Config ConfigModuleJson::loadConfig()
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
Config ConfigModuleJson::parseJsonConfig(const JsonDocument &jsonObject)
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
void ConfigModuleJson::copyToJsonConfig(const Config &config, JsonDocument &doc)
{
    doc[DISABLE_TIME_ID] = config.disableTime.toString();
    doc[ENABLE_TIME_ID] = config.enableTime.toString();
    doc[SET_LED_COLOR_ID] = config.setLedColor;
    doc[BRIGHTNESS_CORRECTION_ID] = config.brightnessCorrection;
}
