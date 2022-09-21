//
// Created by Jonas Lauener on 09.09.18.
//

#ifndef Settings_H
#define Settings_H

const int PANEL_WIDTH = 11; // 11 pixel x 10 pixel matrix of leds
const int PANEL_HEIGHT = 10;
const int PIXEL_COUNT = PANEL_WIDTH * PANEL_HEIGHT;

const int LIGHT_SENSOR_PIN = A0;
const int MAXIMUM_LIGHT_VALUE = 400; // tune for desired behaviour of ambient light adaption

const long TIME_UPDATE_INTERVAL = 10;     // in seconds
const long CLOCK_UPDATE_INTERVAL = 86400; // in seconds

const String NTP_SERVER_NAME = "fr.pool.ntp.org";
const TimeChangeRule TIME_CHANGE_RULE_DST = {"ETE", Last, Sun, Mar, 2, 120}; // Daylight time = UTC+2 hours
const TimeChangeRule TIME_CHANGE_RULE_STD = {"HIVER", Last, Sun, Oct, 3, 60};   // Standard time = UTC+1 hours
const Timezone LOCAL_TIMEZONE(TIME_CHANGE_RULE_DST, TIME_CHANGE_RULE_STD);

const String DEVICE_NAME = "QlockTwo";

const char *const CONFIG_FILE_PATH = "/config.json";

const int LED_COLORS_SIZE = 4;

const RgbColor LED_COLORS[] = {
    RgbColor(63),
    RgbColor(255, 0, 0),
    RgbColor(0, 255, 0),
    RgbColor(0, 0, 255)};

// remoteDebug
#define HOST_NAME "remotedebug"

// Board especific libraries
#if defined ESP8266 || defined ESP32
// Use mDNS ? (comment this do disable it)
#define USE_MDNS true
// Arduino OTA (uncomment this to enable)
//#define USE_ARDUINO_OTA true
#else
// RemoteDebug library is now only to Espressif boards,
// as ESP32 and ESP82266,
// If need for another WiFi boards,
// please add an issue about this
// and we will see if it is possible made the port for your board.
// access: https://github.com/JoaoLopesF/RemoteDebug/issues
#error "The board must be ESP8266 or ESP32"
#endif // ESP

//////// Libraries
#if defined ESP8266
// Includes of ESP8266
#include <ESP8266WiFi.h>
#ifdef USE_MDNS
#include <DNSServer.h>
#include <ESP8266mDNS.h>
#endif
#elif defined ESP32
// Includes of ESP32
#include <WiFi.h>
#ifdef USE_MDNS
#include <DNSServer.h>
#include "ESPmDNS.h"
#endif
#endif // ESP

#endif // Settings_H
