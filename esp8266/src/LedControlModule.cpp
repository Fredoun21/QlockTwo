//
//  LedControlModule.cpp
//  esp8266
//
//  Created by <author> on 10/02/2018.
//
//

#include "LedControlModule.h"

LedControlModule::LedControlModule(NeoTopology<MyPanelLayout> _topo) : topo(_topo) {}
LedControlModule::~LedControlModule() {}

/**
 * Configurez les LED dans la bibliothèque NeoPixel.
 * @param _pixelStrip NeoPixelBusType object
 */
void LedControlModule::setup(NeoPixelBusType *_pixelStrip)
{
    pixelStrip = _pixelStrip;
    pixelStrip->Begin();
    pixelStrip->Show();
};

/**
 * Afficher l'heure avec des LEDs.
 * @param simpleTime Heure à afficher.
 * @param ledColor Facultatif, définissez la couleur de la LED.
 */
void LedControlModule::showTime(const SimpleTime &simpleTime, const RgbColor &ledColor)
{
    pixelStrip->ClearTo(RgbColor(0));
    enableLedWords(simpleTime, ledColor);
    // Serial.println("LedControlModule::showTime");

    // int minuteDots = simpleTime.getMinute() % 5;
    // enableMinuteDots(minuteDots, ledColor);
    pixelStrip->Show();
};

/**
 * Montrez que l'horloge est en mode de configuration Wifi, en allemand, affichez le mot "Funk".
 * @param ledColor Facultatif, définissez la couleur de la LED.
 */
void LedControlModule::showConfigWifi(const RgbColor &ledColor)
{
    pixelStrip->ClearTo(RgbColor(0));
    enableLedWord(&WORD_WIFI, RgbColor(220, 118, 51));
    pixelStrip->Show();
}

/**
 * Activer les mots de temps.
 * @param simpleTime Heure à afficher.
 * @param ledColor Facultatif, définissez la couleur de la LED.
 */
void LedControlModule::enableLedWords(const SimpleTime &simpleTime, const RgbColor &ledColor)
{
    // Serial.println("\nLedControlModule::enableLedWords");
    enableLedWord(&PREFIX_IL, ledColor);
    enableLedWord(&PREFIX_EST, ledColor);
    int fiveMinutes = simpleTime.getMinute() / 5;
    // Serial.printf("\nMinute: %i", fiveMinutes);

    switch (fiveMinutes)
    {
    case 0: // HEURE PILE
        // enableLedWord(&SUFFIX_HEURE, ledColor);
        // Serial.println("HEURE");
        break;
    case 1: // 5 MINUTES
        enableLedWord(&MINUTE_CINQ, ledColor);
        // Serial.println("CINQ MINUTES");
        break;
    case 2: // 10 MINUTES
        enableLedWord(&MINUTE_DIX, ledColor);
        // Serial.println("DIX MINUTES");
        break;
    case 3: // ET QUART
        enableLedWord(&INFIX_ET, ledColor);
        enableLedWord(&MINUTE_QUART, ledColor);
        // Serial.println("ET QUART");
        break;
    case 4: // 20 MINUTES
        enableLedWord(&MINUTE_VINGT, ledColor);
        // Serial.println("VINGT MINUTES");
        break;
    case 5: // 25 MINUTES
        enableLedWord(&MINUTE_VINGTCINQ, ledColor);
        // Serial.println("VINGT-CINQ MINUTES");
        break;
    case 6: // ET DEMI
        enableLedWord(&INFIX_ET, ledColor);
        enableLedWord(&MINUTE_DEMI, ledColor);
        // Serial.println("ET DEMI");
        break;
    case 7: // 35 MINUTES
        enableLedWord(&INFIX_MOINS, ledColor);
        enableLedWord(&MINUTE_VINGTCINQ, ledColor);
        // Serial.println("MOINS VINGT-CINQ");
        break;
    case 8: // 40 MINUTES
        enableLedWord(&INFIX_MOINS, ledColor);
        enableLedWord(&MINUTE_VINGT, ledColor);
        // Serial.println("MOINS VINGT");
        break;
    case 9: // 45 MINUTES
        enableLedWord(&INFIX_MOINS, ledColor);
        enableLedWord(&INFIX_LE, ledColor);
        enableLedWord(&MINUTE_QUART, ledColor);
        // Serial.println("MOINS LE QUART");
        break;
    case 10: // 50 MINUTES
        enableLedWord(&INFIX_MOINS, ledColor);
        enableLedWord(&MINUTE_DIX, ledColor);
        // Serial.println("MOINS DIX");
        break;
    case 11: // 55 MINUTES
        enableLedWord(&INFIX_MOINS, ledColor);
        enableLedWord(&MINUTE_CINQ, ledColor);
        // Serial.println("MOINS CINQ");
        break;
    }

    if (fiveMinutes < 5)
    {
        int hourIndex = (simpleTime.getHour() + 11) % 12;
        // Serial.printf("Heure: %i\n", hourIndex);
        enableLedWord(&HOURS[hourIndex], ledColor);
        enableLedWord(&SUFFIX_HEURE, ledColor);
    }
    else
    {
        int hourIndex = (simpleTime.getHour()) % 12;
        // Serial.printf("Heure: %i\n", hourIndex);
        enableLedWord(&HOURS[hourIndex], ledColor);
        enableLedWord(&SUFFIX_HEURE, ledColor);
    }
};

/**
 * Activer un seul LedWord sur la matrice LED.
 * @param ledWord
 * @param ledColor
 */
void LedControlModule::enableLedWord(const LedWord *ledWord, const RgbColor &ledColor)
{
    // Serial.printf("\nMot %c de %i caractères.", ledWord, ledWord->getLength());
    for (int j = 0; j < ledWord->getLength(); j++)
    {
        pixelStrip->SetPixelColor(topo.Map(ledWord->getFirstPixelX() + j, ledWord->getFirstPixelY()), ledColor);
    }
}

/**
 * Activer les petits points dans les bords.
 * @param subMinute Commence à partir de 1.
 * @param ledColor
 */
void LedControlModule::enableMinuteDots(int subMinute, const RgbColor &ledColor)
{
    for (int i = 1; i <= subMinute; i++)
    {
        int j = 110 + ((i + 2) % 4);
        pixelStrip->SetPixelColor(j, ledColor);
    }
}

/**
 * Eteindre toutes les LEds.
 */
void LedControlModule::disableLeds()
{
    pixelStrip->ClearTo(RgbColor(0));
    pixelStrip->Show();
};