#include "LEDManager.h"

LEDManager::LEDManager()
{
    blinkTimer = 0;
    FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
    FastLED.setBrightness(20);
}

void LEDManager::changeColor(int r, int g, int b)
{
    FastLED.showColor(getColor(r, g, b));
}

void LEDManager::blinkColor(int r, int g, int b, unsigned long currentTime)
{
    if (currentTime - blinkTimer > BLINK_INTERVAL)
    {
        blinkTimer = currentTime;
        changeColor(r, g, b);
    }
    else
    {
        changeColor(0, 0, 0);
    }
}

void LEDManager::updateLED(unsigned long currentTime, float voltage)
{
    if (voltage < 4.8)
    { // Battery low
        blinkColor(255, 0, 0, currentTime);
    }
    else
    {
        blinkColor(0, 255, 0, currentTime);
    }
}

uint32_t LEDManager::getColor(uint8_t r, uint8_t g, uint8_t b)
{
    return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}
