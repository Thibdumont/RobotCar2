#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include "FastLED.h"

#define PIN_RBGLED 4
#define NUM_LEDS 1
#define BLINK_INTERVAL 500

class LEDManager
{
public:
    LEDManager();
    void updateLED(unsigned long, float);
    void changeColor(int, int, int);
    void blinkColor(int, int, int, unsigned long);

private:
    CRGB leds[NUM_LEDS];
    unsigned long blinkTimer;
    uint32_t getColor(uint8_t, uint8_t, uint8_t);
};

#endif
