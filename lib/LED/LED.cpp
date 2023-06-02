#include "LED.h"

unsigned long blinkTimer = 0;
CRGB leds[NUM_LEDS];

uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
    return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

void initLED()
{
    FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
    FastLED.setBrightness(20);
}

void changeColor(int r, int g, int b)
{
    FastLED.showColor(Color(r, g, b));
}

void blinkColor(int r, int g, int b, unsigned long currentTime)
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

void handleLED(unsigned long currentTime, float voltage)
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