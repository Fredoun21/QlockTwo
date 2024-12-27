/**
@file main.cpp
**/

#include <Arduino.h>
#include <Ticker.h>
#include <ElegantOTA.h>
#if defined(AVEC_REMOTE_DEVICE)
#include <RemoteDebug.h> // Remote debug over WiFi - not recommended for production, only for development
#endif
#include "AmbientLightModule.h"
#include "ClockModule.h"
#include "ConfigModuleJson.h"
#include "LedControlModule.h"
#include "Settings.h"
#include "SimpleTime.h"
#include "WifiModule.h"
#ifdef AVEC_APDS9960
#include "GestureModule.h"
#endif

// using namespace ace_button;

//-----------------------------------------------------
// Member Variables
//-----------------------------------------------------
NeoTopology<MyPanelLayout> topo(PANEL_WIDTH, PANEL_HEIGHT); // declaration de la matrice de LEDs

LedControlModule ledControlModule(topo); // déclaration

NeoPixelBusType pixelStrip(PIXEL_COUNT); // déclaration

ConfigModuleJson configModuleJson(CONFIG_FILE_PATH); // declaration gestion fichier JSON

ESP8266WebServer server(80);

WifiModule wifiModule(DEVICE_NAME);

ClockModule clockModule(Wire, LOCAL_TIMEZONE, NTP_SERVER_NAME); // déclaration module RTC

#ifdef AVEC_APDS9960
GestureModule gestureModule(PIN_INTERRUPT);
#endif

AmbientLightModule ambientLight(LIGHT_SENSOR_PIN, MAXIMUM_LIGHT_VALUE);

#if defined(AVEC_REMOTE_DEVICE)
RemoteDebug Debug;
#endif

// AceButton buttonOne(new ButtonConfig());
// AceButton buttonTwo(new ButtonConfig());
// AceButton buttonThree(new ButtonConfig());
// AceButton buttonFour(new ButtonConfig());

unsigned long lastClockUpdate = 0;
unsigned long lastShowTime = 0;
unsigned long lastLightUpdate = 0;
bool showTimeDisabled = false;
uint32_t mTimeSeconds = 0;
int currentLedColorId = 0;
RgbColor currentLedColor = LED_COLORS[currentLedColorId];
Config configJson;

//-----------------------------------------------------
// Function Declarations
//-----------------------------------------------------
// void handleButtonOneEvent(AceButton *, uint8_t, uint8_t);
// void handleButtonTwoEvent(AceButton *, uint8_t, uint8_t);
// void handleButtonThreeEvent(AceButton *, uint8_t, uint8_t);
// void handleButtonFourEvent(AceButton *, uint8_t, uint8_t);
// void setButtonConfig(ButtonConfig *buttonConfig, ButtonConfig::EventHandler eventHandler);
// void setupButtons();

void updateClock();
void updateLedColor(int month);
void showTime();
void configModeCallback(WiFiManager *myWiFiManager);
void saveConfigCallback();

//-----------------------------------------------------
// Function Definitions
//-----------------------------------------------------
/**
 * Setup all Modules
 */
void setup()
{
    Serial.begin(115200);
    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println("\nDebut de l'installation.");
    Serial.println("Chargement configuration:");

    // --------------------------------------------- Lecture fichier de configuration
    configModuleJson.setup();
    configJson = configModuleJson.loadConfig();

    // --------------------------------------------- Configuration LED strip
    currentLedColorId = configJson.setLedColor;
    updateLedColor(currentLedColorId);
    ledControlModule.setup(&pixelStrip);
    ledControlModule.showConfigWifi();

// --------------------------------------------- Démarrage capteur de gestes
#ifdef AVEC_APDS9960
    gestureModule.setup();
#endif

    // setupButtons();

    // --------------------------------------------- Configuration Wifi
    wifiModule.setup(configModeCallback, saveConfigCallback);
    // wifiModule.reset();
    wifiModule.connect();

    server.on("/", []()
              { server.send(200, "text/plain", "Hi! I am QlockTwo."); });

    // --------------------------------------------- Démarrage serveur Elegant OTA
    ElegantOTA.begin(&server); // Start ElegantOTA
    server.begin();
    Serial.println("HTTP server started");

    // Register host name in WiFi and mDNS
    String hostNameWifi = HOST_NAME;
    hostNameWifi.concat(".local");
    WiFi.hostname(hostNameWifi);

#if defined(AVEC_REMOTE_DEVICE)
    // Use the MDNS
    if (MDNS.begin(HOST_NAME))
    {
        Serial.print("* MDNS responder started. Hostname -> ");
        Serial.println(HOST_NAME);
    }
    MDNS.addService("telnet", "tcp", 23);

    // Initialize RemoteDebug
    Debug.begin(HOST_NAME);         // Initialize the WiFi server
    Debug.setResetCmdEnabled(true); // Enable the reset command
    Debug.showProfiler(true);       // Profiler (Good to measure times, to optimize codes)
    Debug.showColors(true);         // Colors
    // End off setup

    Serial.println("* Arduino RemoteDebug Library");
    Serial.println("*");
    Serial.print("* WiFI connected. IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("*");
    Serial.println("* Please use the telnet client (telnet for Mac/Unix or putty and others for Windows)");
    Serial.println("* or the RemoteDebugApp (in browser: http://joaolopesf.net/remotedebugapp)");
    Serial.println("*");
    Serial.println("* This sample will send messages of debug in all levels.");
    Serial.println("*");
    Serial.println("* Please try change debug level in client (telnet or web app), to see how it works");
    Serial.println("*");

#endif

    // --------------------------------------------- Gestion de l'horloge
    clockModule.setup(); // Démarrage RTC
    updateClock();
    lastClockUpdate = millis(); // Mémoire compteur temps MAJ horloge

    showTime();
    lastShowTime = millis();

    Serial.println("Configuration terminée. \n");
}

/**
 * Main Loop
 */
void loop()
{
    int month = 0;
    //  MAJ horloge toutes les 24h
    if ((millis() - lastClockUpdate) > (CLOCK_UPDATE_INTERVAL * 1000))
    {
        updateClock();
        lastClockUpdate = millis(); // Nouveau cycle de 24h
    }

    // MAJ affichage toutes les 10s
    if ((millis() - lastShowTime) > (TIME_UPDATE_INTERVAL * 1000) && !showTimeDisabled)
    {
        month = clockModule.getMonth();
        // updateLedColor(10);
        updateLedColor(month);
        showTime();
        lastShowTime = millis(); // Nouveau cycle de 10s
        mTimeSeconds++;
#ifdef AVEC_APDS9960
        gestureModule.checkProximity();
        gestureModule.checkColor();
#endif
    }

#ifdef AVEC_APDS9960
    gestureModule.checkGesture();
#endif

    // server OTA
    server.handleClient();

#if defined(AVEC_REMOTE_DEVICE)
    // RemoteDebug handle
    Debug.handle();
#endif

    // Give a time for ESP
    yield();
}

/**
 * Est appelé lorsque WiFiManager passe en mode de configuration
 * @param myWiFiManager
 */
void configModeCallback(WiFiManager *myWiFiManager)
{
    Serial.println("\n En mode configuration");

    showTimeDisabled = true;
}

/**
 * Est appelé lorsque WifiManager à des paramètres personnalisés ont été définis ET qu'une connexion a été établie.
 */
void saveConfigCallback()
{
    Serial.println("Save callback.");
    configJson.enableTime = wifiModule.getEnableTime();
    configJson.disableTime = wifiModule.getDisableTime();
    configModuleJson.saveConfig(configJson);

    showTimeDisabled = false;
    showTime();
}

/**
 * Obtenez l'heure de RTC et mettez-la à jour si elle n'est pas correcte à partir de NTP.
 * Afficher uniquement l'heure si elle est liée à la plage horaire activée.
 */
void showTime()
{
    Serial.println("disableTime: " + configJson.disableTime.toString());
    Serial.println("enableTime: " + configJson.enableTime.toString());

    if (!clockModule.isDateTimeValid())
    {
        updateClock();
    }

    const SimpleTime st = clockModule.getLocalSimpleTime();

    if (configJson.disableTime == configJson.enableTime ||
        !(((configJson.disableTime > configJson.enableTime) && (configJson.disableTime <= st && configJson.enableTime < st)) ||
          ((configJson.disableTime < configJson.enableTime) && (configJson.disableTime <= st && configJson.enableTime > st))))
    {
        Serial.println("\tShow Time: " + st.toString());

        ledControlModule.showTime(st, currentLedColor);
    }
    else
    {
        Serial.println("\tShow Time: LED DISABLED");
        ledControlModule.disableLeds();
    }
}

/**
 * Connection au Wifi et mise à jour RTC via NTP.
 */
void updateClock()
{
    Serial.println("\nConnection au wifi et mise à jour de l'horloge.");
    if (!wifiModule.isConnected())
    {
        wifiModule.connect();
    }

    clockModule.update();
}

/**
 * Appliquez la lumière ambiante en atténuant currentLedColor.
 */
void updateLedColor(int month)
{
    currentLedColor = LED_COLORS[month];
    // currentLedColor = LED_COLORS[currentLedColorId];
    //     int darken = 255 - ambientLight.getBrightness();
    //     Serial.printf("\nBrightness: %i\n", darken);
    //     currentLedColor.Darken(darken);
}

/**
 * Handle clicks of 1. button from left.
 * @param button
 * @param eventType
 * @param buttonState
 */
// void handleButtonOneEvent(AceButton *button, uint8_t eventType,
//                           uint8_t buttonState)
// {
//     switch (eventType)
//     {
//     case AceButton::kEventClicked:
//         Serial.println("Button One Clicked");
//         currentLedColorId = (currentLedColorId + 1) % LED_COLORS_SIZE;

//         configJson.setLedColor = currentLedColorId;
//         configModuleJson.saveConfig(configJson);

//         updateLedColor();
//         showTime();
//         break;
//     case AceButton::kEventLongPressed:
//         Serial.println("Button One Long Press");
//         if (!showTimeDisabled)
//         {
//             Serial.println("Disable LED");
//             ledControlModule.disableLeds();
//             showTimeDisabled = true;
//         }
//         else
//         {
//             Serial.println("Enable LED");
//             showTime();
//             showTimeDisabled = false;
//         }
//         break;
//     }
// }

// /**
//  * Handle clicks of 2. button from left.
//  * @param button
//  * @param eventType
//  * @param buttonState
//  */
// void handleButtonTwoEvent(AceButton *button, uint8_t eventType,
//                           uint8_t buttonState)
// {
//     switch (eventType)
//     {
//     case AceButton::kEventClicked:
//         Serial.println("Button Two Clicked");
//         if (ambientLight.getBrightnessCorrection() > -9)
//         {
//             ambientLight.setBrightnessCorrection(ambientLight.getBrightnessCorrection() - 1);
//             Serial.println("Brightness Correction: " + ambientLight.getBrightnessCorrection());
//         };
//         // Serial.println("Current Brightness (out of 255):");
//         // Serial.println(ambientLight.getBrightness());
//         // Serial.println("with User Brightness Correction [-9 9]:");
//         // Serial.println(ambientLight.getBrightnessCorrection());
//         updateLedColor();
//         showTime();
//         break;
//     case AceButton::kEventLongPressed:
//         Serial.println("Button Two Long Press");
//         ambientLight.setBrightnessCorrection(0);
//         // Serial.println("User Brightness Correction Reset");
//         // Serial.println("Current Brightness (out of 255):");
//         // Serial.println(ambientLight.getBrightness());
//         updateLedColor();
//         showTime();
//         break;
//     }
// }

// /**
//  * Handle clicks of 3. button from left.
//  * @param button
//  * @param eventType
//  * @param buttonState
//  */
// void handleButtonThreeEvent(AceButton *button, uint8_t eventType,
//                             uint8_t buttonState)
// {
//     switch (eventType)
//     {
//     case AceButton::kEventClicked:
//         Serial.println("Button Three Clicked");
//         if (ambientLight.getBrightnessCorrection() < 9)
//         {
//             ambientLight.setBrightnessCorrection(ambientLight.getBrightnessCorrection() + 1);
//             Serial.println("Brightness Correction: " + ambientLight.getBrightnessCorrection());
//         };
//         // Serial.println("Current Brightness (out of 255):");
//         // Serial.println(ambientLight.getBrightness());
//         // Serial.println("with User Brightness Correction [-9 9]:");
//         // Serial.println(ambientLight.getBrightnessCorrection());
//         updateLedColor();
//         showTime();
//         break;
//     case AceButton::kEventLongPressed:
//         Serial.println("Button Three Long Press");
//         break;
//     }
// }

// /**
//  * Handle clicks of 4. button from left.
//  * @param button
//  * @param eventType
//  * @param buttonState
//  */
// void handleButtonFourEvent(AceButton *button, uint8_t eventType,
//                            uint8_t buttonState)
// {
//     switch (eventType)
//     {
//     case AceButton::kEventClicked:
//         Serial.println("Button Four Clicked");
//         updateClock();
//         break;
//     case AceButton::kEventLongPressed:
//         Serial.println("Button Four Long Press");
//         wifiModule.reset();
//         delay(1000);
//         wifiModule.connect();
//         break;
//     }
// }
