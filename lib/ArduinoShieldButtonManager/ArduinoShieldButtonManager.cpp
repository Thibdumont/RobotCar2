#include "ArduinoShieldButtonManager.h"

ArduinoShieldButtonManager::ArduinoShieldButtonManager(LEDManager *ledManager)
{
    this->ledManager = ledManager;
    lastButtonRegisterTime = millis();
    wifiSoftApMode = false;
    init();
}

void ArduinoShieldButtonManager::init()
{
    pinMode(ARDUINO_SHIELD_BUTTON_PIN, INPUT_PULLUP);
    attachPinChangeInterrupt(ARDUINO_SHIELD_BUTTON_PIN, staticGetButtonValue, FALLING);
}

void ArduinoShieldButtonManager::getButtonValue()
{
    if (millis() - lastButtonRegisterTime > ARDUINO_SHIELD_BUTTON_PRESS_MIN_INTERVAL)
    {
        lastButtonRegisterTime = millis();
        wifiSoftApMode = !wifiSoftApMode;
        if (wifiSoftApMode)
        {
            ledManager->changeColor(255, 0, 0);
        }
        else
        {
            ledManager->changeColor(0, 255, 0);
        }
    }
}

void ArduinoShieldButtonManager::test()
{
}
