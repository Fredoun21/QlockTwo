//
//  GestureModule.h
//  esp8266
//
//  Created by Fred W on 20/11/2024.
//
//

#ifndef GestureModule_H
#define GestureModule_H

#include <stdio.h>
#include <Arduino_APDS9960.h>

/**
 * GestureModule gère la détetction des gestes pour le réglage de l'horloge
 * et la luminiosité ambiante pour régalge des Leds
 */
class GestureModule
{
public:
    GestureModule(int _intPin);
    ~GestureModule();
    void setup();
    void checkProximity();
    void checkGesture();
    void checkColor();

private:
    int intPin;
    int proximity = 0;
    int r = 0, g = 0, b = 0;
    unsigned long lastUpdate = 0;

protected:
};

#endif /* Apds_9960_h */