#include "LEDManager.h"

LEDManager::LEDManager(TimeManager *timeManager, VoltageManager *voltageManager)
{
    this->timeManager = timeManager;
    this->voltageManager = voltageManager;
    blinkTimer = 0;
    FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
    FastLED.setBrightness(1);
}

void LEDManager::changeColor(int r, int g, int b)
{
    FastLED.showColor(getColor(r, g, b));
}

void LEDManager::blinkColor(int r, int g, int b)
{
    if (timeManager->getLoopTime() - blinkTimer > BLINK_INTERVAL)
    {
        blinkTimer = timeManager->getLoopTime();
        changeColor(r, g, b);
    }
    else
    {
        changeColor(0, 0, 0);
    }
}

void LEDManager::updateLED()
{
    if (voltageManager->getVoltage() < 4.8)
    { // Battery low
        blinkColor(255, 0, 0);
    }
    else
    {
        blinkColor(0, 255, 0);
    }
}

uint32_t LEDManager::getColor(uint8_t r, uint8_t g, uint8_t b)
{
    return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}
