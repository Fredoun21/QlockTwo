//
// Enregistrez et chargez la configuration vers/depuis le SPIFF de l'ESP.
//

#ifndef ESP8266_CONFIGMODULEJSON_H
#define ESP8266_CONFIGMODULEJSON_H

#include "LittleFS.h"

#include "SimpleTime.h"
#include <ArduinoJson.h>
#include <RemoteDebug.h>

static const String ENABLE_TIME_ID = "enable_time";
static const String DISABLE_TIME_ID = "disable_time";
static const String SET_LED_COLOR_ID = "set_led_color";
static const String BRIGHTNESS_CORRECTION_ID = "brightness_correction";

struct Config
{
    SimpleTime enableTime;
    SimpleTime disableTime;
    int setLedColor;
    int brightnessCorrection;
};

/**
 * Enregistrez et chargez la configuration vers/depuis le SPIFF de l'ESP.
 */
class ConfigModuleJson
{
public:
    ConfigModuleJson(String _configFilePath);
    ~ConfigModuleJson();
    void setup();
    bool saveConfig(const Config &config);
    const Config loadConfig();

private:
    const String configFilePath;
    Config parseJsonConfig(const JsonDocument &jsonObject);
    void copyToJsonConfig(const Config &config, JsonDocument &doc);

protected:
};

#endif // ESP8266_CONFIGMODULE_H
