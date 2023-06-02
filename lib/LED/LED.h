#include "FastLED.h"

#define PIN_RBGLED 4
#define NUM_LEDS 1
#define BLINK_INTERVAL 500

uint32_t Color(uint8_t, uint8_t, uint8_t);

void initLED();
void handleLED(unsigned long, float);
void changeColor(int, int, int);
void blinkColor(int, int, int, unsigned long);
