//
// Gestion de la lumière ambiante avec photo-résistance
//

#ifndef ESP8266_AMBIENTLIGHTMODULE_H
#define ESP8266_AMBIENTLIGHTMODULE_H

class AmbientLightModule
{
private:
    int pinNumber;
    int brightnessCorrection = 0;
    int storedData[3] = {0, 0, 0};
    int deadBand = 5;
    int maxLightValue;
    int brightnessLevel[10];

public:
    AmbientLightModule(int _pinNumber, int _maxLightValue);
    ~AmbientLightModule();
    void setBrightnessCorrection(int correction);
    int getBrightnessCorrection();
    int getBrightness();
};

#endif // ESP8266_AMBIENTLIGHTMODULE_H
