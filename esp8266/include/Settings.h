//
// Created by Jonas Lauener on 09.09.18.
//

#ifndef Settings_H
#define Settings_H

const int PANEL_WIDTH = 11; // 11 pixel x 10 pixel matrix of leds
const int PANEL_HEIGHT = 10;
const int PIXEL_COUNT = PANEL_WIDTH * PANEL_HEIGHT;

const int PIN_INTERRUPT = 4; // entrée d'interruption pour capteur de mouvement APDS9960
const int LIGHT_SENSOR_PIN = A0;
const int MAXIMUM_LIGHT_VALUE = 400; // tune for desired behaviour of ambient light adaption

const long TIME_UPDATE_INTERVAL = 10;     // in seconds
const long CLOCK_UPDATE_INTERVAL = 86400; // in seconds

const String NTP_SERVER_NAME = "fr.pool.ntp.org";
const TimeChangeRule TIME_CHANGE_RULE_DST = {"ETE", Last, Sun, Mar, 2, 120};  // Daylight time = UTC+2 hours
const TimeChangeRule TIME_CHANGE_RULE_STD = {"HIVER", Last, Sun, Oct, 3, 60}; // Standard time = UTC+1 hours
const Timezone LOCAL_TIMEZONE(TIME_CHANGE_RULE_DST, TIME_CHANGE_RULE_STD);

const String DEVICE_NAME = "QlockTwo";

const char *const CONFIG_FILE_PATH = "/config.json";

const int LED_COLORS_SIZE = 12;

const RgbColor LED_COLORS[] = {
    RgbColor(248, 248, 255), // Ghostwhite - #F8F8FF - Janvier
    RgbColor(199, 21, 133),  // Mediumvioletred - #C71585 - Février
    RgbColor(0, 255, 127),   // Springgreen - #00FF7F - Mars
    RgbColor(255, 250, 205), // Lemonchiffon - #FFFACD - Avril
    RgbColor(255, 182, 193), // Lightpink - #FFB6C1 - Mai
    RgbColor(135, 206, 235), // Skyblue - #87CEEB - Juin
    RgbColor(255, 99, 71),   // Tomato - #FF6347 - Juillet
    RgbColor(255, 215, 0),   // Gold - #FFD700 - Août
    RgbColor(1, 215, 88),    // VertEmeraude - #01D758 - Septembre
    RgbColor(244, 164, 96),  // Sandybrown - #F4A460 - Octobre
    RgbColor(169, 169, 169), // Darkgrey  - #A9A9A9 - Novembre
    RgbColor(27, 50, 177),   // Firebrick - #1b32b1 - Décembre

};

#undef AVEC_REMOTE_DEVICE
// remoteDebug
#if defined(AVEC_REMOTE_DEVICE)
#define HOST_NAME "remotedebug"
#else
#define HOST_NAME "Qlocktwo"
#endif

#undef AVEC_APDS9960
// #define AVEC_APDS9960

#endif // Settings_H
