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
    uint8_t getWifiSoftApMode();

private:
    LEDManager *ledManager;
    uint8_t wifiSoftApMode;
    int lastButtonState;
    void init();
};

#endif