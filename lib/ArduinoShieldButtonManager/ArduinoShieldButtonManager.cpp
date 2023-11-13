#include "ArduinoShieldButtonManager.h"

ArduinoShieldButtonManager::ArduinoShieldButtonManager(LEDManager *ledManager)
{
    this->ledManager = ledManager;
    lastButtonState = LOW;
    wifiSoftApMode = 0;
    pinMode(ARDUINO_SHIELD_BUTTON_PIN, INPUT_PULLUP);
    ledManager->changeColor(0, 255, 0);
}

void ArduinoShieldButtonManager::detectPress()
{
    int buttonState = digitalRead(ARDUINO_SHIELD_BUTTON_PIN);
    if (buttonState != lastButtonState)
    {
        lastButtonState = buttonState;
        if (buttonState == LOW)
        {
            wifiSoftApMode = !wifiSoftApMode;
            if (wifiSoftApMode)
            {
                ledManager->changeColor(0, 0, 255);
            }
            else
            {
                ledManager->changeColor(0, 255, 0);
            }
        }
    }
}

uint8_t ArduinoShieldButtonManager::getWifiSoftApMode()
{
    return wifiSoftApMode;
}