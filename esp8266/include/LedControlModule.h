//
//  LedControlModule.h
//  esp8266
//
//  Created by <author> on 10/02/2018.
//
//

#ifndef LedControlModule_H
#define LedControlModule_H

#include <stdio.h>
#include <NeoPixelBus.h>
#include <RtcDateTime.h>
#include "LedWord.h"
#include "LedMapping.h"
#include "SimpleTime.h"

typedef NeoPixelBus<NeoGrbFeature, Neo800KbpsMethod> NeoPixelBusType;
typedef RowMajorAlternating180Layout MyPanelLayout;

/**
 * Contrôlez les LED et affichez les mots et les points des minutes.
 */
class LedControlModule
{
public:
    LedControlModule(NeoTopology<MyPanelLayout> _topo);
    ~LedControlModule();
    void setup(NeoPixelBusType *_pixelStrip);
    void disableLeds();
    void showTime(const SimpleTime &simpleTime, const RgbColor &ledColor = RgbColor(255));
    void showConfigWifi(const RgbColor &ledColor = RgbColor(255));

private:
    NeoPixelBusType *pixelStrip;
    NeoTopology<MyPanelLayout> topo;
    void enableLedWords(const SimpleTime &simpleTime, const RgbColor &ledColor);
    void enableLedWord(const LedWord *ledWord, const RgbColor &ledColor);
    void enableMinuteDots(int subMinute, const RgbColor &ledColor);

protected:
};

#endif /* LedControlModule_h */
