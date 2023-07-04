#ifndef ARDUINO_SHIELD_BUTTON_MANAGER_H
#define ARDUINO_SHIELD_BUTTON_MANAGER_H

#include <Arduino.h>
#include "LEDManager.h"

#define ARDUINO_SHIELD_BUTTON_PIN 2

class ArduinoShieldButtonManager
{
public:
    ArduinoShieldButtonManager(LEDManager *ledManager);
    void detectPress();
    boolean getWifiSoftApMode();

private:
    LEDManager *ledManager;
    boolean wifiSoftApMode;
    int lastButtonState;
    void init();
};

#endif