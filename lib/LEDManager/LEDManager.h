#ifndef LED_MANAGER_H
#define LED_MANAGER_H

#include "FastLED.h"
#include "TimeManager.h"
#include "VoltageManager.h"

#define PIN_RBGLED 4
#define NUM_LEDS 1
#define BLINK_INTERVAL 500

class LEDManager
{
public:
    LEDManager(TimeManager *, VoltageManager *);
    void changeColor(int, int, int);
    void blinkColor(int, int, int);

private:
    TimeManager *timeManager;
    VoltageManager *voltageManager;
    CRGB leds[NUM_LEDS];
    unsigned long blinkTimer;
    uint32_t getColor(uint8_t, uint8_t, uint8_t);
};

#endif
