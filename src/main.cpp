#include <Arduino.h>
#include "LED.h"

#define VOL_MEASURE_PIN A3

unsigned long currentTime = 0;

float voltage = 0;
unsigned long lastVoltageMeasure = 0;

// put function declarations here:

void voltageInit();
void updateVoltage();

void setup()
{
  Serial.begin(9600);

  initLED();
  voltageInit();
}

void loop()
{
  currentTime = millis();

  handleLED(currentTime, voltage);
  updateVoltage();
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