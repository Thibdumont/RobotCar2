#include <Arduino.h>
#include "FastLED.h"
#define PIN_RBGLED 4
#define NUM_LEDS 1
#define VOL_MEASURE_PIN A3
#define BLINK_INTERVAL 500

unsigned long currentTime = 0;
unsigned long blinkTimer = 0;
float voltage = 0;
unsigned long lastVoltageMeasure = 0;

CRGB leds[NUM_LEDS];
uint32_t Color(uint8_t r, uint8_t g, uint8_t b)
{
  return (((uint32_t)r << 16) | ((uint32_t)g << 8) | b);
}

// put function declarations here:
void handleLED();
void changeColor(int, int, int);
void blinkColor(int, int, int);

void voltageInit();
void updateVoltage();

void setup()
{
  Serial.begin(9600);
  FastLED.addLeds<NEOPIXEL, PIN_RBGLED>(leds, NUM_LEDS);
  FastLED.setBrightness(20);
  voltageInit();
}

void loop()
{
  currentTime = millis();

  handleLED();
  updateVoltage();
}

void handleLED()
{
  if (voltage < 4.8)
  { // Battery low
    blinkColor(255, 0, 0);
  }
  else
  {
    blinkColor(0, 255, 0);
  }
}

void changeColor(int r, int g, int b)
{
  FastLED.showColor(Color(r, g, b));
}

void blinkColor(int r, int g, int b)
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

void voltageInit()
{
  pinMode(VOL_MEASURE_PIN, INPUT);
}

void updateVoltage()
{
  if (currentTime - lastVoltageMeasure > 1000)
  {
    lastVoltageMeasure = currentTime;
    voltage = (analogRead(VOL_MEASURE_PIN) * 5) * ((10 + 1.5) / 1.5) / 1024; // Read voltage value
                                                                             // float voltage = (analogRead(VOL_MEASURE_PIN) * 0.0375);
    voltage = voltage + (voltage * 0.08);
    Serial.print(" Voltage : ");
    Serial.println(voltage, 3);
  }
}