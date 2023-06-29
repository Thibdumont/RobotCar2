#ifndef ARDUINO_SHIELD_BUTTON_MANAGER_H
#define ARDUINO_SHIELD_BUTTON_MANAGER_H

#include <Arduino.h>
#include "LEDManager.h"
#include "PinChangeInterrupt.h"

#define ARDUINO_SHIELD_BUTTON_PIN 2
#define ARDUINO_SHIELD_BUTTON_PRESS_MIN_INTERVAL 500

class ArduinoShieldButtonManager
{
public:
    ArduinoShieldButtonManager(LEDManager *ledManager);
    static void staticGetButtonValue(ArduinoShieldButtonManager *thisInstance)
    {
        ArduinoShieldButtonManager *self = reinterpret_cast<ArduinoShieldButtonManager *>(thisInstance);
        self->getButtonValue();
    }
    void test();

private:
    LEDManager *ledManager;
    boolean wifiSoftApMode;
    unsigned long lastButtonRegisterTime;
    void init();
    void getButtonValue();
};

#endif