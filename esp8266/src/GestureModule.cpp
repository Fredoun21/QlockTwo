//
//  GestureModule.cpp
//  esp8266
//
//  Created by Fred W on 20/11/2024.
//
//

#include "GestureModule.h"
#include <Arduino.h>

/// @brief
/// @param _intPin
GestureModule::GestureModule(int _intPin) : intPin(_intPin) {}

/// @brief
GestureModule::~GestureModule() {}

/// @brief
void GestureModule::setup()
{
    if (!APDS.begin())
    {
        Serial.println("Error initializing APDS-9960 sensor.");
        while (true)
            ; // Stop forever
    }
    else
    {
        Serial.println("APDS-9960 init OK.");
    }
};

/// @brief
/// Check if a proximity reading is available.
void GestureModule::checkProximity()
{

    if (APDS.proximityAvailable())
    {
        proximity = APDS.readProximity();
    }
    Serial.printf("Distance = %i\n", proximity);
};

/// @brief
/// Check if a gesture reading is available
void GestureModule::checkGesture()
{
    if (APDS.gestureAvailable())
    {
        int gesture = APDS.readGesture();
        switch (gesture)
        {
        case GESTURE_UP:
            Serial.println("Détection d'un geste UP");
            break;

        case GESTURE_DOWN:
            Serial.println("Détection d'un geste DOWN ");
            break;

        case GESTURE_LEFT:
            Serial.println("Détection d'un geste LEFT ");
            break;

        case GESTURE_RIGHT:
            Serial.println("Détection d'un geste RIGHT ");
            break;

        default:
            // Ignore
            break;
        }
    }
};

/// @brief
/// Check if a color reading is available
void GestureModule::checkColor()
{
    if (APDS.colorAvailable())
    {
        APDS.readColor(r, g, b);
    }
    Serial.printf("R, G, B: %03i,%03i,%03i\n", r, g, b);
};